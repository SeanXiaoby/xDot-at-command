#include "ctype.h"
#include "CommandFactory.h"
#include "CommandTerminal.h"
#include "Command.h"
#include "MTSLog.h"
#include "ChannelPlan.h"
#include <cstdarg>
#include <deque>
#include "mts_at_version.h"

#if defined(TARGET_XDOT_L151CC)
#include "xdot_low_power.h"
#endif

#if defined(TARGET_MTS_MDOT_F411RE)
const char CommandTerminal::banner[] = "\r\n\nMultiTech Systems LoRa XBee Module\r\n\n";
#else
const char CommandTerminal::banner[] = "\r\n\nMultiTech Systems LoRa xDot Module\r\n\n";
#endif
const char CommandTerminal::helpline[] = "Enter '?' for help\r\n";

const char CommandTerminal::newline[] = "\r\n";

// Command error text...
const char CommandTerminal::command_error[] = "Command not found!\r\n";

// Response texts...
const char CommandTerminal::help[] = "\r\nHelp\r\n";
const char CommandTerminal::cmd_error[] = "Invalid command\r\n";
const char CommandTerminal::connect[] = "\r\nCONNECT\r\n";
const char CommandTerminal::no_carrier[] = "\r\nNO CARRIER\r\n";
const char CommandTerminal::done[] = "\r\nOK\r\n";
const char CommandTerminal::error[] = "\r\nERROR\r\n";

// Escape sequence...
const char CommandTerminal::escape_sequence[] = "+++";

mts::ATSerial* CommandTerminal::_serialp = NULL;
mDot* CommandTerminal::_dot = NULL;

CommandTerminal::RadioEvent* CommandTerminal::_events = new RadioEvent();

static bool serial_data_mode = false;
static bool peer_to_peer = false;
static bool command_processing = false;
static bool urc_enabled = false;

std::string CommandTerminal::_errorMessage = "";

static uint8_t _battery_level = 0xFF;

static uint8_t f_data[252]; //max payload 242 plus 10 bytes for format
static uint32_t _rxAddress = 0;
static uint32_t _rxFcnt = 0;

#if defined(TARGET_MTS_MDOT_F411RE)
DigitalOut _packet_rx_pin(D12);
DigitalOut _join_status_pin(A2);
#else
DigitalOut _packet_rx_pin(GPIO1);
DigitalOut _join_status_pin(GPIO0);
#endif

void CommandTerminal::setErrorMessage(const char* message) {
    _errorMessage.assign(message);
}

void CommandTerminal::setErrorMessage(const std::string& message) {
    _errorMessage.assign(message);
}

CommandTerminal::CommandTerminal(mts::ATSerial& serial) :
  _serial(serial),
  _mode(mDot::COMMAND_MODE),
  _sleep_standby(true),
#if defined(TARGET_MTS_MDOT_F411RE)
  _xbee_on_sleep(XBEE_ON_SLEEP),
#else
  _xbee_on_sleep(GPIO2),
#endif
  _autoOTAEnabled(false)
{
    _serialp = &serial;
}

void free_mem() {
    // In order to get free mem within RTOS
    // we need to get the main thread's stack pointer
    // and subtract it with the top of the heap
    // ------+-------------------+   Last Address of RAM (INITIAL_SP)
    //       | Scheduler Stack   |
    //       +-------------------+
    //       | Main Thread Stack |
    //       |         |         |
    //       |         v         |
    //       +-------------------+ <- bottom_of_stack/__get_MSP()
    // RAM   |                   |
    //       |  Available RAM    |
    //       |                   |
    //       +-------------------+ <- top_of_heap
    //       |         ^         |
    //       |         |         |
    //       |       Heap        |
    //       +-------------------+ <- __end__ / HEAP_START (linker defined var)
    //       | ZI                |
    //       +-------------------+
    //       | ZI: Shell Stack   |
    //       +-------------------+
    //       | ZI: Idle Stack    |
    //       +-------------------+
    //       | ZI: Timer Stack   |
    //       +-------------------+
    //       | RW                |
    // ------+===================+  First Address of RAM
    //       |                   |
    // Flash |                   |
    //

    uint32_t bottom_of_stack = __get_MSP();
    char* top_of_heap =  (char *) malloc(sizeof(char));
    uint32_t diff = bottom_of_stack - (uint32_t) top_of_heap;

    free((void *) top_of_heap);

    CommandTerminal::Serial()->writef("%lu bytes\r\n", diff);
}

void CommandTerminal::init() {
    _dot->setEvents(_events);
    _serial.rxClear();
    _serial.txClear();
}

#if MTS_CMD_TERM_VERBOSE
void CommandTerminal::printHelp() {
    const char* name = NULL;
    const char* text = NULL;
    const char* desc = NULL;
    const char* tab = "\t";

    std::string header("Command");
    header.append(tab);
    header.append(tab);
    header.append("Name");
    header.append(tab);
    header.append(tab);
    header.append(tab);
    header.append("Description");

    write(newline);
    write(header.c_str());
    write(newline);
    write(newline);

    Command *cmd = NULL;
    for (int i = 0; i < CommandFactory::NUMBER_OF_CMDS; i++) {
        cmd = CommandFactory::Create(static_cast<CommandFactory::CmdId_t>(i));
        name = cmd->name();
        text = cmd->text();
        desc = cmd->desc();
        write(text);
        if (strlen(text) < 8)
            write(tab);
        write(tab);
        write(name);
        if (strlen(name) < 8)
            write(tab);
        if (strlen(name) < 16)
            write(tab);
        write(tab);
        write(desc);
        write(newline);

        delete cmd;
    }

    write(newline);
}
#endif

bool CommandTerminal::writeable() {
    return _serialp->writeable();
}

bool CommandTerminal::readable() {
    return _serialp->readable();
}

char CommandTerminal::read() {
    char ch;
    _serialp->read(ch);
    return ch;
}

void CommandTerminal::write(const char* message) {
    while (!writeable())
        ;
    _serialp->write(message, strlen(message));
}

void CommandTerminal::writef(const char* format, ...) {
    char buff[512];

    va_list ap;
    va_start(ap, format);
    int size = vsnprintf(buff, 256, format, ap);
    while (!writeable())
        ;
    _serialp->write(buff, size);
    va_end(ap);
}

void CommandTerminal::serialLoop() {
    Timer serial_read_timer;
    std::vector<uint8_t> serial_buffer;
    std::vector<uint8_t> data;
    std::chrono::milliseconds timeout(0);
    std::chrono::milliseconds elapsed_time_ms;

    serial_read_timer.start();

    if (_dot->getStartUpMode() == mDot::SERIAL_MODE) {
        _xbee_on_sleep = GPIO_PIN_SET;

        timeout = std::chrono::milliseconds(_dot->getWakeDelay());
        elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(serial_read_timer.elapsed_time());

        // wait for timeout or start of serial data
        while (!readable() && elapsed_time_ms < timeout && !_serialp->escaped()) {
            ThisThread::sleep_for(2ms);
            elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(serial_read_timer.elapsed_time());
        }
    }

    if (!readable() && _events->SendAck()) {
        logDebug("SERIAL NOT READABLE and ACK REQUESTED");
        goto L_SEND;
    }

    if (readable() && !_serialp->escaped()) {

        serial_read_timer.reset();
        timeout = std::chrono::milliseconds(_dot->getWakeTimeout());

        while (true) {
            elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(serial_read_timer.elapsed_time());
            if ((elapsed_time_ms >= timeout) ) {

                break;
            }
            if (serial_buffer.size() >= _dot->getNextTxMaxSize())  {
                break;
            }
            if  (_serialp->escaped())  {
                break;
            }
            if (readable()) {
                serial_buffer.push_back(read());
                serial_read_timer.reset();
            } else {
                ThisThread::sleep_for(5ms);
            }
        }

        serial_read_timer.stop(), serial_read_timer.reset();

        if (!serial_buffer.empty()) {
            if (_dot->getStartUpMode() == mDot::SERIAL_MODE)
                 _xbee_on_sleep = GPIO_PIN_RESET;

L_SEND:
            // wait for any duty cycle limit to expire
            while (_dot->getNextTxMs() > 0 && !_serialp->escaped()) {
                ThisThread::sleep_for(10ms);
            }

            if (_dot->getIsIdle()) {
                logDebug("Received serial data, sending out radio.");
                if(_dot->getRxOutput() == mDot::EXTENDED || _dot->getRxOutput() == mDot::EXTENDED_HEX) {
                    formatPacketSDSend(serial_buffer);
                }

                if (_dot->send(serial_buffer, false) != mDot::MDOT_OK) {
                    logDebug("Send failed.");
                    // If the data should be tossed after send failure, clear buffer
                    if (_dot->getSerialClearOnError()) {
                        serial_buffer.clear();
                    }
                } else {

                    // wait for send to finish
                    do {
                        ThisThread::sleep_for(50ms);
                    } while (!_dot->getIsIdle() && !_serialp->escaped());

                    // call recv to wait for any packet before sending again
                    if (!_serialp->escaped()) {
                        data.clear();
                        _dot->recv(data);
                    }

                    // Clear the serial buffer if send is success
                    serial_buffer.clear();

                    // In P2P and Class B & C mode pending data will be sent automatically without uplink
                    if (peer_to_peer != true && _dot->getClass() == "A") {
                        if (_dot->getDataPending()) {
                            logDebug("Data is pending");
                            goto L_SEND;
                        }
                        if (_dot->getAckRequested()) {
                            logDebug("Ack requested");
                            goto L_SEND;
                        }
                        if (_dot->hasMacCommands()) {
                            logDebug("Pending MAC answers");
                            goto L_SEND;
                        }
                    }
                }
            } else {
                logDebug("Radio is busy, cannot send.\r\n");
                ThisThread::sleep_for(10ms);
            }

        } else {
            logDebug("No data received from serial to send.\r\n");
            ThisThread::sleep_for(10ms);
        }
    }

    if (!_serialp->readable() && _dot->getStartUpMode() == mDot::SERIAL_MODE && !_serialp->escaped()) {
        sleep(true);
    }

    if (_serialp->escaped()) {
        _serialp->clearEscaped();
        _serialp->rxClear();
        serial_data_mode = false;
        _mode = mDot::COMMAND_MODE;
        logDebug("Exit Serial Mode");
        write(done);
        return;
    }

    if (!_dot->getNetworkJoinStatus()) {
        serial_data_mode = false;
        _mode = mDot::COMMAND_MODE;
        logDebug("Exit Serial Mode");
        write(no_carrier);
        return;
    }
}

bool CommandTerminal::autoJoinCheck() {

    std::string escape_buffer;
    int sleep = 1000;
    Timer tmr;
    tmr.start();
    int cnt = 0;

    while (!_dot->getNetworkJoinStatus()) {
        if (!serial_data_mode) {
            write("\r\nJoining network... ");
        }
        logInfo("Joining network... ");

        if (_dot->getNextTxMs() > 0) {
            if (!serial_data_mode) {
                writef("\r\nWaiting %lu s before next join attempt\r\n", _dot->getNextTxMs() / 1000);
            }
            logInfo("Waiting %lu s before next join attempt", _dot->getNextTxMs() / 1000);

            tmr.reset();
            while (_dot->getNextTxMs() > 0 && !_serial.escaped()) {
                osDelay(20);
            }
        }

        if (!_serial.escaped() && _dot->joinNetworkOnce() == mDot::MDOT_OK) {
            if (!serial_data_mode) {
                write("Network Joined\r\n");
                write(done);
            }
            logInfo("Network Joined");
            return false;
        }

        if (!serial_data_mode) {
            write("Network Join failed\r\n");
            write(error);
        }
        logInfo("Network Join failed");

        if (!_serial.escaped() && _dot->getFrequencySubBand() != 0 && _dot->getJoinRetries() > 0 && cnt++ > _dot->getJoinRetries()) {
            cnt = 0;

            if (lora::ChannelPlan::IsPlanFixed(_dot->getFrequencyBand())) {
                uint8_t band = ((_dot->getFrequencySubBand()) % 8) + 1;
                logWarning("Join retries exhausted, switching to sub band %u", band);
                _dot->setFrequencySubBand(band);
            }
        }

        tmr.reset();
        while (tmr.read_ms() < sleep && !_serial.escaped()) {
            osDelay(10);
        }

        if (_serial.escaped()) {
            _serial.clearEscaped();
            serial_data_mode = false;
            _mode = mDot::COMMAND_MODE;
            if (!serial_data_mode) {
                write("Join Canceled\r\n");
                write(done);
            }
            logInfo("Join Canceled\r\n");
            return true;
        }
    }

    return false;
}

#define CMD_DEFS_COUNT  (13)
#define CMD_DEFS_VARIANT_SIZE  (4)
#define CMD_DEFS_LABEL_SIZE  (7)

struct CommandDefinition {
    const char label[CMD_DEFS_LABEL_SIZE];
    const char var[CMD_DEFS_VARIANT_SIZE];
    uint8_t max_args;
    CommandFactory::CmdId_t id;
};

// Table of commands handled locally
static const CommandDefinition cmd_defs[CMD_DEFS_COUNT] = {
    {"", "", 0, CommandFactory::eAT},
    {"E", "?01", 0, CommandFactory::eATE},
    {"V", "?01", 0, CommandFactory::eATVERBOSE},
    {"&K", "?03", 0, CommandFactory::eATK},
    {"+URC", "?", 1, CommandFactory::eURC},
    {"+LW", "?", 0, CommandFactory::eLW},
    {"+SD", "?", 0, CommandFactory::eSD},
    {"&WP", "", 0, CommandFactory::eATWP},
    {"&W", "", 0, CommandFactory::eATW},
    {"+SS", "", 0, CommandFactory::eSS},
    {"+DP", "?", 0, CommandFactory::eDP},
    {"+SLEEP", "", 1, CommandFactory::eSLEEP},
    {"+MEM", "?", 0, CommandFactory::eMEM}
};



void CommandTerminal::start() {

    char ch;
    bool running = true;
    bool echo = _dot->getEcho();
    std::string command;
#if defined(TARGET_MTS_MDOT_F411RE)
    std::deque<std::string> history;
    int history_index = -1;
#endif
    std::vector<std::string> args;
    bool join_canceled = false;

    _autoOTAEnabled = _dot->getJoinMode() == mDot::AUTO_OTA;

    if (_dot->getStartUpMode() == mDot::SERIAL_MODE || CommandTerminal::Dot()->getTestModeEnabled()) {

        serial_data_mode = true;
        _mode = mDot::SERIAL_MODE;

        std::string escape_buffer;
        char ch;

        if (!_dot->getStandbyFlag()) {
            // wake up from power-on/reset

            int escape_timeout = 1000;
            Timer tmr;
            Timer escape_tmr;

            // wait one second for possible escape by user pressing '+' key
            tmr.reset();
            tmr.start();
            escape_tmr.reset();
            escape_tmr.start();
            while (tmr.read_ms() < escape_timeout) {
                if (_serial.readable()) {
                    _serial.read(&ch, 1);
                    escape_buffer += ch;
                }

                if (escape_buffer.find("+") != std::string::npos) {
                    logInfo("Escape detected");
                    join_canceled = true;
                    serial_data_mode = false;
                    CommandTerminal::Dot()->setTestModeEnabled(false);
                    _mode = mDot::COMMAND_MODE;
                    command.clear();
                    break;
                }

                if (escape_tmr.read_ms() > escape_timeout)
                    escape_buffer.clear();

                osDelay(1);
            }
        }

        if (_dot->getTestModeEnabled()) {
            Fota::getInstance()->enable(false);
        }

        if ((_mode == mDot::SERIAL_MODE || CommandTerminal::Dot()->getTestModeEnabled()) && !_dot->getNetworkJoinStatus() && _dot->getJoinMode() == mDot::OTA) {
            if (_dot->joinNetworkOnce() != mDot::MDOT_OK) {
                serial_data_mode = false;
                _mode = mDot::COMMAND_MODE;

                logWarning("Start Up Mode set to SERIAL_MODE, but join failed.");
                _serial.writef("Network Not Joined\r\n");
                _serial.writef(error);
            }
        }
    }

    if (CommandTerminal::Dot()->getTestModeEnabled() && _dot->getNetworkJoinStatus()) {
        while (CommandTerminal::Dot()->getTestModeEnabled() && !_events->PacketReceived) {
            std::vector<uint8_t> data;
            CommandTerminal::Dot()->send(data, false);
            osDelay(5000);
        }
    }

    if (_dot->getJoinMode() == mDot::PEER_TO_PEER) {
        peer_to_peer = true;
        Fota::getInstance()->enable(false);
        CommandTerminal::Dot()->clearMacCommands();
        CommandTerminal::Dot()->setTxDataRate(CommandTerminal::Dot()->getTxDataRate());
    } else {
        peer_to_peer = false;
    }
    //Run terminal session
    while (running) {

        if (_events != NULL && CommandTerminal::Dot()->getTestModeEnabled() && !_serial.escaped()) {
            if (_dot->getNextTxMs() > 0) {
                osDelay(2000);
            } else if (!_dot->getNetworkJoinStatus() && _dot->getJoinMode() == mDot::OTA) {
                if (_dot->joinNetworkOnce() != mDot::MDOT_OK) {
                    serial_data_mode = false;
                    _mode = mDot::COMMAND_MODE;

                    logWarning("Start Up Mode set to SERIAL_MODE, but join failed.");
                    _serial.writef("Network Not Joined\r\n");
                    _serial.writef(error);
                }
            } else if (_events->PacketReceived) {
                _events->handleTestModePacket();
                _events->PacketReceived =  false;

            } else {
                while (CommandTerminal::Dot()->getTestModeEnabled() && !_events->PacketReceived  && !_serial.escaped()) {
                    std::vector<uint8_t> data;
                    CommandTerminal::Dot()->send(data, false);
                    osDelay(5000);
                }
            }
            continue;
        }

        // wait for input to reduce at command idle current
        while (!readable() || _mode == mDot::SERIAL_MODE) {
            if (!join_canceled && _autoOTAEnabled) {
                join_canceled = autoJoinCheck();
                if (join_canceled)
                    command.clear();
            }

            if (!_autoOTAEnabled || (!join_canceled && _autoOTAEnabled)) {
                switch (_mode) {
                    case mDot::SERIAL_MODE:
                        // signal wakeup, read serial and output to radio
                        serialLoop();
                        continue;
                        break;
                    default:
                        break;
                }
            }

            ch = '\0';

            if (_mode != mDot::SERIAL_MODE) {
                ThisThread::sleep_for(10ms);
            }
            _serial.escaped();
        }

        // read characters
        if (readable()) {
            ch = read();

            if (ch == '\b' || ch == 0x7f) {
                if (!command.empty()) {
                    writef("\b \b");
                    command.erase(command.size() - 1);
                }
                continue;
            } else if (ch == 0x1b || ch == 0x09) {
                osDelay(20);
                // catch escape sequence, or tab
                char ch1 = 0x00, ch2 = 0x00;

                if (readable()) {
                    ch1 = read();
                    if (readable())
                        ch2 = read();

#if defined(TARGET_MTS_MDOT_F411RE)
                    if (ch1 == 0x5b && ch2 == 0x41) {
                        // up key
                        for (size_t i = 0; i < command.size() + 1; i++) {
                            writef("\b \b");
                        }
                        if (history.size() > 0) {
                            if (++history_index >= int(history.size() - 1))
                                history_index = history.size() - 1;

                            command = history[history_index];
                            writef("%s", history[history_index].c_str());
                        } else {
                            command.clear();
                        }
                    } else if (ch1 == 0x5b && ch2 == 0x42) {

                        // down key
                        for (size_t i = 0; i < command.size() + 1; i++) {
                            writef("\b \b");
                        }

                        if (--history_index < 0) {
                            history_index = -1;
                            command.clear();
                        } else {
                            command = history[history_index];
                            writef("%s", history[history_index].c_str());
                        }
                    }
#endif
                }
                while (readable())
                    read();
                continue;
            } else {
                command += ch;
            }

            // echo chars if enabled
            if (echo && !(ch == '\r' || ch == '\n'))
                writef("%c", ch);
        }

        // look for end of command line
        if (command.find("\n") != std::string::npos || command.find("\r") != std::string::npos) {
            // remove new line or cr character
            command.erase(command.size() - 1);
            write("\r"); // match standard modem output
            write(newline);
        } else {
            continue;
        }

        // trim whitespace from command
        mts::Text::trim(command, "\r\n\t ");

        if (command.size() < 1) {
            command.clear();
            continue;
        }

        _packet_rx_pin = 0;
        command_processing = true;

        // parse command and args
        args.clear();

        // find first '=' character
        size_t delim_index = command.find("=");
        if (delim_index != std::string::npos) {
            args.push_back(command.substr(0, delim_index));
        } else {
            // find first ' ' character
            delim_index = command.find(" ");
            if (delim_index != std::string::npos) {
                args.push_back(command.substr(0, delim_index));
            } else {
                args.push_back(command);
            }
        }

        if (delim_index != std::string::npos) {
            std::vector<std::string> params = mts::Text::split(command.substr(delim_index + 1), ",");
            args.insert(args.end(), params.begin(), params.end());
        }

        args[0] = mts::Text::toUpper(args[0]);
        bool handled = false;

        // print help
        if ((args[0].find("?") == 0 || args[0].find("HELP") == 0) && args.size() == 1) {
#if MTS_CMD_TERM_VERBOSE
            printHelp();
#endif
            command.clear();
            handled = true;
        } else if (args[0].find("AT") == 0) {
            const CommandDefinition* def = NULL;    // Command to handle if matched
            char variant = '\0';                    // Variant character
            for (int d = 0; (d < CMD_DEFS_COUNT) && (def == NULL); ++d) {
                size_t label_size = 2 + strlen(cmd_defs[d].label);
                if (args[0].find(cmd_defs[d].label) == 2) {
                    // Label found following "AT"
                    for (int v = 0; v < CMD_DEFS_VARIANT_SIZE; ++v) {
                        if ((args[0][label_size] == cmd_defs[d].var[v]) &&
                            (args[0][label_size] == '\0' || args[0][label_size + 1] == '\0')) {
                            // Check for variant characters following label, this includes a NULL
                            def = &cmd_defs[d];
                            variant = cmd_defs[d].var[v];
                            break;
                        }
                    }
                }
            }
            if (def != NULL) {
                handled = true;
                if (args.size() == 2 && args[1].length() == 1 && args[1][0] == '?') {
                    handled = false;
                } else if (args.size() - 1 > def->max_args) {
#if MTS_CMD_TERM_VERBOSE
                    write("Invalid argument\r\n");
#endif
                    write(error);
                } else {
                    switch (def->id) {
                        case CommandFactory::eAT:
                            write(done);
                            break;
                        case CommandFactory::eATE:
                            if (variant == '1' || variant == '0') {
                                _dot->setEcho(variant == '1');
                                echo = _dot->getEcho();
                            } else {
                                writef("%d\r\n", _dot->getEcho());
                            }
                            write(done);
                            break;
                        case CommandFactory::eATVERBOSE:
                            if (variant == '1' || variant == '0') {
                                _dot->setVerbose(variant == '1');
                            } else {
                                writef("%d\r\n", _dot->getVerbose());
                            }
                            write(done);
                            break;
                        case CommandFactory::eATK:
                            if (variant == '3' || variant == '0') {
                                _dot->setFlowControl(variant == '3');
                            } else {
                                writef("%d\r\n", (_dot->getFlowControl() ? 3 : 0));
                            }
                            write(done);
                            break;
                        case CommandFactory::eURC:
                            if (args.size() == 1) {
                                writef("%d\r\n", urc_enabled);
                                write(done);
                            } else if (args[1].length() == 1) {
                                if (args[1][0] != '?' && args[1][0] != '0' && args[1][0] != '1') {
#if MTS_CMD_TERM_VERBOSE
                                    write("Invalid argument\r\n");
#endif
                                    write(error);
                                } else if (args[1][0] == '?') {
#if MTS_CMD_TERM_VERBOSE
                                    write("(0:disable,1:enable)\r\n");
                                    write(done);
#else
                                    write(error);
#endif
                                } else {
                                    urc_enabled = (args[1][0] == '1');
                                    write(done);
                                }
                            } else {
                                write(error);
                            }
                            break;
                        case CommandFactory::eLW:
                            writef("%s\r\n", _dot->getMACVersion());
                            write(done);
                            break;
                        case CommandFactory::eMEM:
                            free_mem();
                            write(done);
                            break;
                        case CommandFactory::eSD:
                            if (_dot->getNetworkJoinStatus()) {
                                logDebug("Enter Serial Mode");
                                write(connect);
                                serial_data_mode = true;
                                _serialp->clearEscaped();
                                _mode = mDot::SERIAL_MODE;
                            } else {
                                logDebug("Network Not Joined");
                                write("Network Not Joined\r\n");
                                write(error);
                            }
                            break;
                        case CommandFactory::eATW:
                            if (!_dot->saveConfig()) {
#if MTS_CMD_TERM_VERBOSE
                                write("Failed to save to flash");
#endif
                                write(error);
                            } else {
                                write(done);
                            }
                            break;
                        case CommandFactory::eATWP:
                            if (!_dot->saveProtectedConfig()) {
#if MTS_CMD_TERM_VERBOSE
                                write("Failed to save to flash");
#endif
                                write(error);
                            } else {
                                write(done);
                            }
                            break;
                        case CommandFactory::eSS:
                            _dot->saveNetworkSession();
                            write(done);
                            break;
                        case CommandFactory::eDP:
                            writef("%d\r\n",
                                _dot->getDataPending() ||
                                _dot->hasMacCommands() ||
                                _dot->getAckRequested());
                            write(done);
                            break;
                        case CommandFactory::eSLEEP: {
                            bool temp_sleep_standby;
                            bool valid = false;
                            if ((args.size() > 1) && (args[1].length() == 1)) {
                                if ((args[1][0] != '?' && args[1][0] != '0' && args[1][0] != '1')) {
#if MTS_CMD_TERM_VERBOSE
                                    write("Invalid argument\r\n");
#endif
                                    write(error);
                                } else if (args[1][0] == '?') {
#if MTS_CMD_TERM_VERBOSE
                                    write("(0:deepsleep,1:sleep)\r\n");
                                    write(done);
#else
                                    write(error);
#endif
                                } else {
                                    valid = true;
                                    temp_sleep_standby = (args[1][0] == '0');
                                }
                            } else if (args.size() == 1) {
                                valid = true;
                                temp_sleep_standby = _sleep_standby;
                            } else {
                                write(error);
                            }

                            if (valid) {
                                if (!_dot->getIsIdle()) {
                                    write("Dot is not idle\r\n");
                                    write(error);
                                } else {
                                    _sleep_standby = temp_sleep_standby;
#if defined(TARGET_MTS_MDOT_F411RE)
                                    //Read the board ID. If all 0's, it is revision B. This hardware does not support deep sleep.
                                    DigitalIn ID2(PC_4);
                                    DigitalIn ID1(PC_5);
                                    DigitalIn ID0(PD_2);
                                    if(ID2 == 0 && ID1 == 0 && ID0 == 0 && _sleep_standby == 1){
                                        _sleep_standby = 0;
                                        logWarning("This hardware version does not support deep sleep. Using sleep mode instead.");
                                    }
#endif
                                    write(done);
                                    if (_dot->getBaud() < 9600)
                                        osDelay(20);
                                    else if (_dot->getBaud() > 57600)
                                        osDelay(2);
                                    else
                                        osDelay(5);
                                    this->sleep(_sleep_standby);
                                    osDelay(1);
                                }
                            }
                            break;
                        }
                        default:
                            handled = false;
                            // Unhandled command
                            // Will only reach here if the table contains commands that do not have explicit cases
                            break;
                    }
                }
#ifdef MTS_RADIO_DEBUG_COMMANDS
            } else if (args[0].find("AT+TM!") == 0 && args[0].length() == 6) {
                handled = true;
                if ((args.size() > 1) && (args[1].length() == 1)) {
                    if (args[1][0] == '0' || args[1][0] == '1') {
                        _dot->setTestModeEnabled(args[1][0] == '1');
                        _dot->saveConfig();
                        write(done);
                    } else {
                        write(error);
                    }
                } else {
                    writef("%d\r\n", _dot->getTestModeEnabled());
                    write(done);
                }
#endif
            }
        }

        if (!handled) {
            bool found = false;
            bool query = false;

            std::string lookfor = args[0];

#if MTS_CMD_TERM_VERBOSE
            // per command help
            if ((args[0].find("?") == 0 || args[0].find("HELP") == 0)) {
                lookfor = mts::Text::toUpper(args[1]);
            }
#endif
            // trim off any trailing '?' and mark as a query command
            if (args[0].rfind("?") == args[0].length() - 1) {
                query = true;
                lookfor = args[0].substr(0, args[0].length() - 1);
            }

            // search for command
            Command *cmd = NULL;
            for (int i = 0; i < CommandFactory::NUMBER_OF_CMDS; i++) {
                cmd = CommandFactory::Create(static_cast<CommandFactory::CmdId_t>(i));

                // match CMD or CMD? syntax if command is queryable
                if (lookfor == cmd->text() && (!query || (query && cmd->queryable()))) {
                    found = true;
                    _errorMessage.clear();
#if MTS_CMD_TERM_VERBOSE
                    if (args[0] == "HELP") {
                        writef("%s%s", cmd->help().c_str(), newline);
                        write(done);
                    }
                    else if (args.size() > 1 && args[1] == "?") {
                        writef("%s%s", cmd->usage().c_str(), newline);
                        write(done);
                    } else if (!cmd->verify(args)) {
#else
                    if (args.size() > 1 && args[1] == "?") {
                        write(error);
                    } else if (!cmd->verify(args)) {
#endif
                        if (!_errorMessage.empty()) {
                            writef("%s%s", _errorMessage.c_str(), newline);
                        }
                        write(error);
                    } else {
                        _errorMessage.clear();
                        if (cmd->action(args) == 0) {
                            writef("%s", done);
                        } else {
                            // Action was not successful
                            if (_errorMessage.empty()) {
                                // If no error message was set, check for error recorded in mdot
                                std::string dot_error = _dot->getLastError();
                                if (!dot_error.empty()) {
                                    writef("%s%s", dot_error.c_str(), newline);
                                }
                            } else {
                                // Command set an error message
                                writef("%s%s", _errorMessage.c_str(), newline);
                            }
                            write(error);
                        }
                    }
                }

                delete cmd;

                if (found) {
                    break;
                }
            }

            if (!found) {
                write(command_error);
                write(error);
            }
        }


        _join_status_pin = CommandTerminal::Dot()->getNetworkJoinStatus();
        command_processing = false;


#if defined(TARGET_MTS_MDOT_F411RE)
        if (history.size() == 0 || history.front() != command)
            history.push_front(command);
        history_index = -1;
        command.clear();

        while (history.size() > 10)
            history.pop_back();
#else
        command.clear();
#endif
    }
}

void CommandTerminal::sleep(bool standby) {
    _xbee_on_sleep = GPIO_PIN_RESET;

    _serial.rxClear();
    _serial.txClear();

#if defined(TARGET_XDOT_L151CC)
    xdot_save_gpio_state();

    /* GPIO Ports Clock Enable */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // UART1_TX, UART1_RTS & UART1_CTS to analog nopull - RX could be a wakeup source
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // I2C_SDA & I2C_SCL to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // SPI_MOSI, SPI_MISO, SPI_SCK, & SPI_NSS to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // iterate through potential wake pins - leave the configured wake pin alone if one is needed
    if (_dot->getWakePin() != WAKE || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    if (_dot->getWakePin() != GPIO0 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    if (_dot->getWakePin() != GPIO1 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    if (_dot->getWakePin() != GPIO2 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    if (_dot->getWakePin() != GPIO3 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    if (_dot->getWakePin() != UART1_RX || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
#else
    uint32_t portA[6];
    uint32_t portB[6];
    uint32_t portC[6];
    uint32_t portD[6];
    uint32_t portH[6];

    //Save the GPIO state.
    portA[0] = GPIOA->MODER;
    portA[1] = GPIOA->OTYPER;
    portA[2] = GPIOA->OSPEEDR;
    portA[3] = GPIOA->PUPDR;
    portA[4] = GPIOA->AFR[0];
    portA[5] = GPIOA->AFR[1];

    portB[0] = GPIOB->MODER;
    portB[1] = GPIOB->OTYPER;
    portB[2] = GPIOB->OSPEEDR;
    portB[3] = GPIOB->PUPDR;
    portB[4] = GPIOB->AFR[0];
    portB[5] = GPIOB->AFR[1];

    portC[0] = GPIOC->MODER;
    portC[1] = GPIOC->OTYPER;
    portC[2] = GPIOC->OSPEEDR;
    portC[3] = GPIOC->PUPDR;
    portC[4] = GPIOC->AFR[0];
    portC[5] = GPIOC->AFR[1];

    portD[0] = GPIOD->MODER;
    portD[1] = GPIOD->OTYPER;
    portD[2] = GPIOD->OSPEEDR;
    portD[3] = GPIOD->PUPDR;
    portD[4] = GPIOD->AFR[0];
    portD[5] = GPIOD->AFR[1];

    portH[0] = GPIOH->MODER;
    portH[1] = GPIOH->OTYPER;
    portH[2] = GPIOH->OSPEEDR;
    portH[3] = GPIOH->PUPDR;
    portH[4] = GPIOH->AFR[0];
    portH[5] = GPIOH->AFR[1];

    /* GPIO Ports Clock Enable */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Set port A pins to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Set port B pins to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Set port C pins to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // iterate through potential wake pins - leave the configured wake pin alone if one is needed
    // XBEE_DIN - PA3
    // XBEE_DIO2 - PA5
    // XBEE_DIO3 - PA4
    // XBEE_DIO4 - PA7
    // XBEE_DIO5 - PC1
    // XBEE_DIO6 - PA1
    // XBEE_DIO7 - PA0
    // XBEE_SLEEPRQ - PA11

    if (_dot->getWakePin() != XBEE_DIN || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    if (_dot->getWakePin() != XBEE_DIO2 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    if (_dot->getWakePin() != XBEE_DIO3 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

         if (_dot->getWakePin() != XBEE_DIO4 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

     if (_dot->getWakePin() != XBEE_DIO5 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }

     if (_dot->getWakePin() != XBEE_DIO6 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

     if (_dot->getWakePin() != XBEE_DIO7 || _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

     if (_dot->getWakePin() != XBEE_SLEEPRQ|| _dot->getWakeMode() == mDot::RTC_ALARM) {
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

#endif
    _dot->sleep(_dot->getWakeInterval(), _dot->getWakeMode(), standby);

#if defined(TARGET_XDOT_L151CC)
    xdot_restore_gpio_state();
#else
    //Restore the GPIO state.
    GPIOA->MODER = portA[0];
    GPIOA->OTYPER = portA[1];
    GPIOA->OSPEEDR = portA[2];
    GPIOA->PUPDR = portA[3];
    GPIOA->AFR[0] = portA[4];
    GPIOA->AFR[1] = portA[5];

    GPIOB->MODER = portB[0];
    GPIOB->OTYPER = portB[1];
    GPIOB->OSPEEDR = portB[2];
    GPIOB->PUPDR = portB[3];
    GPIOB->AFR[0] = portB[4];
    GPIOB->AFR[1] = portB[5];

    GPIOC->MODER = portC[0];
    GPIOC->OTYPER = portC[1];
    GPIOC->OSPEEDR = portC[2];
    GPIOC->PUPDR = portC[3];
    GPIOC->AFR[0] = portC[4];
    GPIOC->AFR[1] = portC[5];

    GPIOD->MODER = portD[0];
    GPIOD->OTYPER = portD[1];
    GPIOD->OSPEEDR = portD[2];
    GPIOD->PUPDR = portD[3];
    GPIOD->AFR[0] = portD[4];
    GPIOD->AFR[1] = portD[5];

    GPIOH->MODER = portH[0];
    GPIOH->OTYPER = portH[1];
    GPIOH->OSPEEDR = portH[2];
    GPIOH->PUPDR = portH[3];
    GPIOH->AFR[0] = portH[4];
    GPIOH->AFR[1] = portH[5];
#endif

    _serial.rxClear();
    _serial.txClear();
    Fota::getInstance()->fixEventQueue();
}

std::string CommandTerminal::formatPacketData(const std::vector<uint8_t>& data, const uint8_t& format) {
    switch(format) {
        case(mDot::HEXADECIMAL):
            return mts::Text::bin2hexString(data);

        case(mDot::BINARY):
            return std::string(data.begin(), data.end());

        case(mDot::EXTENDED):
            return formatPacket(data, false);

        case(mDot::EXTENDED_HEX):
            return formatPacket(data, true);

        default:
            return "";
    }
}

bool CommandTerminal::waitForEscape(int timeout, mDot* dot, WaitType wait) {
    Timer timer;

    timer.start();
    while (timer.read_ms() < timeout) {

        if (dot != NULL) {
            if (wait == WAIT_SEND && (!dot->getIsTransmitting())) {
                return false;
            }
        }

        if (_serialp != NULL && _serialp->escaped()) {
            _serialp->clearEscaped();
            return true;
        }

        osDelay(10);
    }

    return false;
}

void CommandTerminal::wakeup(void) {
}

void CommandTerminal::RadioEvent::RxDone(uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot) {
    mDotEvent::RxDone(payload, size, rssi, snr, ctrl, slot);
    logDebug("RadioEvent - RxDone");
}

void CommandTerminal::RadioEvent::RxTimeout(uint8_t slot) {
    mDotEvent::RxTimeout(slot);
    _packet_rx_pin = 0;
}

void CommandTerminal::RadioEvent::PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries, uint32_t address, uint32_t fcnt, bool dupRx) {
    mDotEvent::PacketRx(port, payload, size, rssi, snr, ctrl, slot, retries, address, fcnt, dupRx);
    _rxAddress = address;
    _rxFcnt = fcnt;

    if(port == 200 || port == 201 || port == 202) {
        Fota::getInstance()->processCmd(payload, port, size);
        if (CommandTerminal::Dot()->getRxOutput() < mDot::EXTENDED) {
            return;
        }
    }

    _packet_rx_pin = 1;

    if (serial_data_mode && port != 0) {
        if (size > 0) {
            while (!CommandTerminal::Serial()->writeable()) ;
            if (CommandTerminal::Dot()->getRxOutput() >= mDot::EXTENDED) {
                formatPacket(RxPayload, size, CommandTerminal::Dot()->getRxOutput()  == mDot::EXTENDED_HEX);
            } else {
                CommandTerminal::Serial()->write((char*) RxPayload, RxPayloadSize);
            }
        }
        if (!CommandTerminal::Serial()->readable()
            && (_dot->getAckRequested()
                || (peer_to_peer == false && _dot->hasMacCommands()))
            && (peer_to_peer || _dot->getClass() == "C" || _dot->getSettings()->Session.Class == lora::CLASS_B)) {
            _sendAck = true;
        }
    } else if (urc_enabled) {
        if (!command_processing) {
            while (!CommandTerminal::Serial()->writeable()) ;
            if (CommandTerminal::Dot()->getRxOutput() >= mDot::EXTENDED) {
                formatPacket(RxPayload, size, CommandTerminal::Dot()->getRxOutput() == mDot::EXTENDED_HEX);
            } else {
                CommandTerminal::Serial()->write("RECV\r\n", 6);
            }
        }
    }
}

void CommandTerminal::RadioEvent::handleTestModePacket() {
#ifdef MTS_RADIO_DEBUG_COMMANDS
    static uint32_t last_rx_seq = 0;
    bool start_test = false;

    std::string packet = mts::Text::bin2hexString(RxPayload, RxPayloadSize);

        CommandTerminal::Dot()->getSettings()->Test.TestMode = true;

        last_rx_seq = CommandTerminal::Dot()->getSettings()->Session.UplinkCounter;

        uint16_t testDownlinkCounter = 0;
        uint32_t txPeriod = 5000;
        bool testConfirmed = false;

        // init counter
        std::vector<uint8_t> data;
        uint8_t savedPort = CommandTerminal::Dot()->getAppPort();
        uint8_t savedAcks = CommandTerminal::Dot()->getAck();
        bool savedAdr = CommandTerminal::Dot()->getAdr();

        CommandTerminal::Dot()->setAck(0);
        CommandTerminal::Dot()->setAdr(true);
        CommandTerminal::Dot()->setAppPort(224);

        Timer sentTimer;
        sentTimer.start();
        while (CommandTerminal::Dot()->getSettings()->Test.TestMode) {
TEST_START:
            // if (waitingForBeacon && BeaconLocked) {
            //     logInfo("send BeaconRxStatusInd");
            //     data.clear();
            //     data.push_back(0x40);
            //     for (size_t i = 0; i < sizeof(BeaconData); ++i) {
            //         data.push_back(((uint8_t*)&BeaconData)[i]);
            //     }
            // } else

            if (RxPort == 224) {
                data.clear();

                std::string packet = mts::Text::bin2hexString(RxPayload, RxPayloadSize);
                logDebug("Test Mode AppPort : %d", CommandTerminal::Dot()->getAppPort());
                logDebug("Test Mode Packet : %s", packet.c_str());

                switch (RxPayload[0]) {
                    case 0x00: { // PackageVersionReq
                        data.push_back(0x00);
                        data.push_back(0x06);
                        data.push_back(0x01);
                        break;
                    }
                    case 0x01: { // DutResetReq
                        if (RxPayloadSize == 1) {
                            CommandTerminal::Dot()->resetCpu();
                        }
                        break;
                    }
                    case 0x02: { // DutJoinReq
                        if (RxPayloadSize == 1) {
                            CommandTerminal::Dot()->joinNetworkOnce();
                            return;
                        }
                        break;
                    }
                    case 0x03: { // SwitchClassReq
                        std::string cls = "A";
                        if (RxPayload[1] > 0 && RxPayload[1] < 3) {
                            cls = RxPayload[1] == 0x01 ? "B" : "C";
                        }
                        CommandTerminal::Dot()->setClass(cls);
                        break;
                    }
                    case 0x04: { // ADR Enabled/Disable
                        if (RxPayload[1] == 1)
                            CommandTerminal::Dot()->setAdr(true);
                        else
                            CommandTerminal::Dot()->setAdr(false);
                        break;
                    }
                    case 0x05: { // RegionalDutyCycleCtrlReq
                        if (RxPayload[1] == 0)
                            CommandTerminal::Dot()->setDisableDutyCycle(true);
                        else
                            CommandTerminal::Dot()->setDisableDutyCycle(false);

                        break;
                    }
                    case 0x06: { // TxPeriodicityChangeReq
                        if (RxPayload[1] < 2)
                            // 0, 1 => 5s
                            txPeriod = 5000U;
                        else if (RxPayload[1] < 8)
                            // 2 - 7 => 10s - 60s
                            txPeriod = (RxPayload[1] - 1) * 10000U;
                        else if (RxPayload[1] < 11) {
                            // 8, 9, 10 => 120s, 240s, 480s
                            txPeriod = 120 * (1 << (RxPayload[1] - 8)) * 1000U;
                        }
                        break;
                    }
                    case 0x07: { // TxFramesCtrl
                        if (RxPayload[1] == 0) {
                            // NO-OP
                        } else if (RxPayload[1] == 1) {
                            testConfirmed = false;
                            CommandTerminal::Dot()->getSettings()->Network.AckEnabled = 0;
                        } else if (RxPayload[1] == 2) {
                            testConfirmed = true;
                            // if ADR has set nbTrans then use the current setting
                            CommandTerminal::Dot()->getSettings()->Network.AckEnabled = 1;
                            if (CommandTerminal::Dot()->getSettings()->Session.Redundancy == 0) {
                                CommandTerminal::Dot()->getSettings()->Session.Redundancy = 1;
                            }
                        }
                        break;
                    }
                    case 0x08: { // EchoPayloadReq
                        data.push_back(0x08);
                        for (size_t i = 1; i < RxPayloadSize; i++) {
                            data.push_back(RxPayload[i] + 1);
                        }
                        break;
                    }
                    case 0x09: { // RxAppCntReq
                        data.push_back(0x09);
                        data.push_back(testDownlinkCounter & 0xFF);
                        data.push_back(testDownlinkCounter >> 8);
                        break;
                    }
                    case 0x0A: { // RxAppCntResetReq
                        testDownlinkCounter = 0;
                        break;
                    }
                    case 0x20: { // LinkCheckReq
                        CommandTerminal::Dot()->addMacCommand(lora::MOTE_MAC_LINK_CHECK_REQ, 0, 0);
                        break;
                    }
                    case 0x21: { // DeviceTimeReq
                        CommandTerminal::Dot()->addDeviceTimeRequest();
                        break;
                    }
                    case 0x22: { // PingSlotInfo
                        CommandTerminal::Dot()->setPingPeriodicity(RxPayload[1]);
                        CommandTerminal::Dot()->addMacCommand(lora::MOTE_MAC_PING_SLOT_INFO_REQ, RxPayload[1], 0);
                        break;
                    }
                    case 0x7D: { // TxCw
                        uint32_t freq = 0;
                        uint16_t timeout = 0;
                        uint8_t power = 0;

                        timeout = RxPayload[2] << 8 | RxPayload[1];
                        freq = (RxPayload[5] << 16 | RxPayload[4] << 8 | RxPayload[2]) * 100;
                        power = RxPayload[6];

                        CommandTerminal::Dot()->sendContinuous(true, timeout * 1000, freq, power);
                        break;
                    }
                    case 0x7E: { // DutFPort224DisableReq
                        _dot->setTestModeEnabled(false);
                        CommandTerminal::Dot()->getSettings()->Test.TestMode = false;
                        _dot->saveConfig();
                        CommandTerminal::Dot()->resetCpu();
                        break;
                    }
                    case 0x7F: { // DutVersionReq
                        std::string version = AT_APPLICATION_VERSION;
                        int temp = 0;

                        data.push_back(0x7F);
                        int ret = sscanf(&version[0], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MAJOR; // MAJOR
                        ret = sscanf(&version[2], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MINOR; // MINOR
                        ret = sscanf(&version[4], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        if (version.size() > 7) {
                            ret = sscanf(&version[6], "%d", &temp);
                            data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        } else {
                            data.push_back(0); // AT_APP_VERSION_PATCH; // PATCH
                        }
                        version = LW_VERSION;
                        ret = sscanf(&version[0], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MAJOR; // MAJOR
                        ret = sscanf(&version[2], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MINOR; // MINOR
                        ret = sscanf(&version[4], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        if (version.size() > 7) {
                            ret = sscanf(&version[6], "%d", &temp);
                            data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        } else {
                            data.push_back(0); // AT_APP_VERSION_PATCH; // PATCH
                        }
                        version = RP_VERSION;
                        ret = sscanf(&version[0], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MAJOR; // MAJOR
                        ret = sscanf(&version[2], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_MINOR; // MINOR
                        ret = sscanf(&version[4], "%d", &temp);
                        data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        if (version.size() > 7) {
                            ret = sscanf(&version[6], "%d", &temp);
                            data.push_back(temp); // AT_APP_VERSION_PATCH; // PATCH
                        } else {
                            data.push_back(0); // AT_APP_VERSION_PATCH; // PATCH
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }

            do {
                while (std::chrono::duration_cast<std::chrono::milliseconds>(sentTimer.elapsed_time()).count() < txPeriod
                    || CommandTerminal::Dot()->getNextTxMs() > 0) {
                    // if (waitingForBeacon && BeaconLocked) {
                    //     goto TEST_START;
                    // }
                    osDelay(1000);
                }

                sentTimer.reset();

                if (data.size() == 0) {
                    CommandTerminal::Dot()->setAppPort(1);
                    data.push_back(0xFF);
                } else {
                    CommandTerminal::Dot()->setAppPort(224);
                }

                if (CommandTerminal::Dot()->send(data, testConfirmed) == mDot::MDOT_MAX_PAYLOAD_EXCEEDED) {
                    data.clear();
                    RxPort = 0;
                    goto TEST_START;
                }

                if (PacketReceived) {
                    last_rx_seq = CommandTerminal::Dot()->getSettings()->Session.UplinkCounter;
                }

                data.clear();

            } while (!AckReceived && CommandTerminal::Dot()->recv(data) != mDot::MDOT_OK);

            if (AckReceived || (PacketReceived && (RxPort != 0 || RxPayloadSize == 0))) {
                testDownlinkCounter++;
                logDebug("Incremented downlink cnt %d", testDownlinkCounter);
            }

            PacketReceived = false;
            AckReceived = false;
        }
#endif
}

uint8_t CommandTerminal::getBatteryLevel() {
    return _battery_level;
}

void CommandTerminal::setBatteryLevel(uint8_t battery_level) {
    _battery_level = battery_level;
}

void CommandTerminal::formatPacket(uint8_t* payload, uint16_t size, bool hex) {

    if(_dot->getAckRequested()) {
        f_data[0] = 0;
    } else {
        f_data[0] = 1;
    }

    f_data[1] = _rxAddress & 0xFF;
    f_data[2] = (_rxAddress >> 8) & 0xFF;
    f_data[3] = (_rxAddress >> 16) & 0xFF;
    f_data[4] = (_rxAddress >> 24) & 0xFF;
    f_data[5] = _rxFcnt & 0xFF;
    f_data[6] = (_rxFcnt >> 8) & 0xFF;
    f_data[7] = (_rxFcnt >> 16) & 0xFF;
    f_data[8] = (_rxFcnt >> 24) & 0xFF;
    f_data[9] = _events->RxPort;

    for(int i = 0; i < size; i++)
        f_data[i+10] = payload[i];

    if (hex) {
        std::string data = "RECV " + mts::Text::bin2hexString(f_data, size + 10) + "\r\n";
        CommandTerminal::Serial()->write(data.c_str(), data.size());
    } else {
        CommandTerminal::Serial()->write((char*)f_data, size + 10);
    }
}


std::string CommandTerminal::formatPacket(std::vector<uint8_t> payload, bool hex) {

    if(_dot->getAckRequested()) {
        f_data[0] = 0;
    } else {
        f_data[0] = 1;
    }

    f_data[1] = _rxAddress & 0xFF;
    f_data[2] = (_rxAddress >> 8) & 0xFF;
    f_data[3] = (_rxAddress >> 16) & 0xFF;
    f_data[4] = (_rxAddress >> 24) & 0xFF;
    f_data[5] = _rxFcnt & 0xFF;
    f_data[6] = (_rxFcnt >> 8) & 0xFF;
    f_data[7] = (_rxFcnt >> 16) & 0xFF;
    f_data[8] = (_rxFcnt >> 24) & 0xFF;
    f_data[9] = _events->RxPort;

    for(size_t i = 0; i < payload.size(); i++)
        f_data[i+10] = payload.at(i);

    if (hex) {
        return mts::Text::bin2hexString(f_data, payload.size() + 10);
    } else {
        return std::string((char*)f_data, payload.size() + 10);
    }

}



void CommandTerminal::formatPacketSDSend(std::vector<uint8_t> &payload) {
    if (_dot->getRxOutput() == mDot::EXTENDED_HEX) {
        int temp;
        std::vector<uint8_t> converted;

        for (size_t i=0; i < payload.size(); i+=2) {
            if (sscanf((char*)&payload[i], "%2x", &temp) == 1) {
                converted.push_back((uint8_t)temp);
            }
        }

        payload = converted;
    }

    if(payload.size() >= 3) {
        _dot->setAppPort(payload[0]);
        _dot->setRepeat(0);
        _dot->setAck(0);
        switch(payload[1]) {
            case 0:
                _dot->setAck(payload[2]);
                break;
            case 1:
                _dot->setRepeat(payload[2]);
                break;
            default:
                break;
        }
        payload.erase(payload.begin(), payload.begin()+3);
    }

}



CommandTerminal::~CommandTerminal() {
    delete _events;
}
