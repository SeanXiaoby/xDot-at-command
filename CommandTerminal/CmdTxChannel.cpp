#include "CmdTxChannel.h"
#include "ChannelPlan.h"

namespace {
void parseArgs(const std::vector<std::string>& args, int& index, int& frequency, int& datarateRange) {
    sscanf(args[1].c_str(), "%d", &index);
    sscanf(args[2].c_str(), "%d", &frequency);
    sscanf(args[3].c_str(), "%02x", &datarateRange);
}


void printChannel(const char* name, uint32_t channel, uint8_t range_max, uint8_t range_min, int16_t on) {
#if MTS_CMD_TERM_VERBOSE
    if (on != -1) {
        CommandTerminal::Serial()->writef("  %-6s%9d%7X%4X%5d\r\n", name, channel, range_max, range_min, on);
    } else {
        CommandTerminal::Serial()->writef("  %-6s%9d%7X%4X\r\n", name, channel, range_max, range_min);
    }
#else
    CommandTerminal::Serial()->writef("%s,%d,%X,%X,%d\r\n", name, channel, range_max, range_min, on);
#endif
}

void assignName(char* name, int i) {
    if (i > 9) {
        int t = i - (i % 10);
        name[0] = '0' + (t / 10);
        name[1] = '0' + (i - t);
    } else {
        name[0] = i + '0';
        name[1] = '\0';
    }
}

} // namespace

CmdTxChannel::CmdTxChannel() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Channels", "AT+TXCH", "List Tx channel frequencies for sub-band", "<INDEX>,<FREQUENCY>,<DR_RANGE>")
#else
    Command("AT+TXCH")
#endif
{
    _queryable = true;
}

uint32_t CmdTxChannel::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {

        std::vector<uint32_t> channels = CommandTerminal::Dot()->getChannels();
        std::vector<uint8_t> ranges = CommandTerminal::Dot()->getChannelRanges();
        std::vector<uint16_t> mask = CommandTerminal::Dot()->getChannelMask();

        //channelProps props { "\0\0", 0, 0, 0, 0 };
        char name[3] = "\0\0";
        uint32_t channel;
        uint8_t range_max;
        uint8_t range_min;
        int16_t on;

#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::Serial()->writef("Index\tFrequency  DR Max Min  On\r\n");
#endif

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            for (int8_t i = 0; i < 17; i++) {
                channel = channels[i];
                range_max = ranges[i] >> 4;
                range_min = ranges[i] & 0xF;
                if (i == 16) {
                    name[0] = 'R';
                    name[1] = '2';
                    on = -1;
                    if (channels[16] == 0) {
                        range_max = ranges[16];
                        range_min = ranges[168];
                    }
                } else {
                    assignName(name, i);
                    channel = channels[i];
                    if (channels[i] != 0) {
                        on = (mask[0] & (0x1 << i)) ? 1 : 0;
                    } else {
                        on = 0;
                    }
                }
                
                printChannel(name, channel, range_max, range_min, on);
            }
        } else {
            uint8_t sub_band = CommandTerminal::Dot()->getFrequencySubBand();
            if (sub_band > 0) {
                uint8_t offset = (sub_band - 1) * 8;

                for (int i = 0; i < 10; i++) {
                    channel = channels[i];
                    if (i == 9) {
                        name[0] = 'R';
                        name[1] = '2';
                        range_max = ranges[i];
                        range_min = ranges[i];
                        on = -1;
                    } else {
                        if (i == 8) {
                            name[0] = 'U';
                            on = (mask[4] & (0x0001 << (sub_band - 1))) ? 1 : 0;
                        } else {
                            assignName(name, i);
                            if ((sub_band % 2) == 1) {
                                on = (mask[(offset + i)/16] & (0x0001 << i)) ? 1 : 0;
                            } else {
                                on = (mask[(offset + i)/16] & (0x0001 << (i + 8))) ? 1 : 0;
                            }
                        }
                        range_max = ranges[i] >> 4;
                        range_min = ranges[i] & 0xF;
                    }
                    printChannel(name, channel, range_max, range_min, on);
                }
            } else {
                for (size_t i = 0; i < channels.size(); i++) {
                    channel = channels[i];
                    if (i == channels.size() - 1) {
                        name[0] = 'R';
                        name[1] = '2';
                        range_max = ranges[i];
                        range_min = ranges[i];
                        on = -1;
                    } else {
                        assignName(name, i);
                        range_max = ranges[i] >> 4;
                        range_min = ranges[i] & 0xF;
                        on = (mask[i/16] & (0x0001 << (i%16))) ? 1 : 0;
                    }
                    printChannel(name, channel, range_max, range_min, on);
                }
            }
        }
    }

    if (args.size() == 4) {
        int index;
        int frequency;
        int datarateRange;

        parseArgs(args, index, frequency, datarateRange);

        if (CommandTerminal::Dot()->addChannel(index, frequency, datarateRange) != mDot::MDOT_OK) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Failed to add channel");
#endif
            return 1;
        }
    }

    return 0;
}

bool CmdTxChannel::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;


    if (args.size() == 4) {

        int index;
        int frequency;
        int datarateRange;

        parseArgs(args, index, frequency, datarateRange);

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            if (frequency != 0 && (frequency < int(CommandTerminal::Dot()->getMinFrequency()) || frequency > int(CommandTerminal::Dot()->getMaxFrequency()))) {
#if MTS_CMD_TERM_VERBOSE
                char tmp[256];
                sprintf(tmp, "Invalid frequency, expects (0,%lu-%lu)", CommandTerminal::Dot()->getMinFrequency(), CommandTerminal::Dot()->getMaxFrequency());
                CommandTerminal::setErrorMessage(tmp);
#endif
                return false;
            }
        } else {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Fixed channel plans cannot be changed, use AT+FSB or AT+CHM to limit");
#endif
            return false;
        }

        return true;
    } else {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::setErrorMessage("Invalid arguments");
#endif
        return false;
    }

}
