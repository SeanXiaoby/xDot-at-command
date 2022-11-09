#include "CmdSendStringOnInterval.h"
#include "CommandTerminal.h"

CmdSendStringOnInterval::CmdSendStringOnInterval()
: Command("Send On Interval", "AT+SENDI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Sends supplied packet data on interval between sends, output any received packets (escape sequence: +++)",
#else
    "",
#endif
    "(1-2147483647) ms,(string:242)") {

}

uint32_t CmdSendStringOnInterval::action(const std::vector<std::string>& args) {

    int32_t code;
    int interval;
    std::string* text = new std::string();

    sscanf(args[1].c_str(), "%d", &interval);

    for (size_t i = 2; i < args.size(); i++) {
        text->append(args[i]);
        if (i != args.size() - 1)
            text->append(",");
    }

    std::vector<uint8_t> data(text->begin(), text->end());
    std::vector<uint8_t> mt;
    delete text;

    while (true) {
        if (CommandTerminal::Dot()->getJoinMode() == mDot::AUTO_OTA) {
            Timer timer;
            timer.start();
            while (!CommandTerminal::Dot()->getNetworkJoinStatus()) {
                CommandTerminal::Serial()->writef("Joining network... ");
                if (CommandTerminal::Dot()->joinNetworkOnce() == mDot::MDOT_OK) {
                    CommandTerminal::Serial()->writef("Network Joined\r\n");
                } else {
                    CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->getLastError().c_str());
                }

                if (CommandTerminal::waitForEscape(CommandTerminal::Dot()->getNextTxMs())) {
                    return 0;
                }
            }
        } else if (!CommandTerminal::Dot()->getNetworkJoinStatus()) {
            CommandTerminal::setErrorMessage(mDot::getReturnCodeString(mDot::MDOT_NOT_JOINED));
            return 1;
        }

        if ((code = CommandTerminal::Dot()->send(data, CommandTerminal::Dot()->getTxWait())) != mDot::MDOT_OK) {
            std::string error = CommandTerminal::Dot()->getLastError();

            if (code == mDot::MDOT_INVALID_PARAM) {
                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
                return 1;
            } else {
                CommandTerminal::Serial()->writef("%s\r\n", error.c_str());
            }
        }

        if (code == mDot::MDOT_OK) {
            if (CommandTerminal::waitForEscape(3000, CommandTerminal::Dot(), CommandTerminal::WAIT_SEND)) {
                return 0;
            }

            if (CommandTerminal::Dot()->getTxWait() || CommandTerminal::Dot()->getAck() > 0) {
                std::vector<uint8_t> rx_data;

                if (CommandTerminal::Dot()->recv(rx_data) == mDot::MDOT_OK) {
                    if (!rx_data.empty()) {
                        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::formatPacketData(rx_data, CommandTerminal::Dot()->getRxOutput()).c_str());
                    }
                }
            } else if (!CommandTerminal::Dot()->getTxWait()) {
                CommandTerminal::Dot()->cancelRxWindow();
            }
        }

        if (CommandTerminal::waitForEscape(interval)) {
            return 0;
        }
    }
}

bool CmdSendStringOnInterval::verify(const std::vector<std::string>& args) {
    if (args.size() >= 3) {
        int interval;
        size_t size = 0;

        if (sscanf(args[1].c_str(), "%d", &interval) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (interval < 1 || interval > INT_MAX-1) {
            CommandTerminal::setErrorMessage("Invalid interval, expects (1-2147483646) ms");
            return false;
        }

        for (size_t i = 2; i < args.size() - 1; i++)
            size += args[i].size() + 1;
        if (size > 242) {
            CommandTerminal::setErrorMessage("Invalid packet, expects (string:242)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
