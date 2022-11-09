#include "CmdTxChannel.h"
#include "ChannelPlan.h"

CmdTxChannel::CmdTxChannel()
: Command("Tx Channels", "AT+TXCH",
#if defined(TARGET_MTS_MDOT_F411RE)
    "List Tx channel frequencies for sub-band",
#else
    "",
#endif
    "<INDEX>,<FREQUENCY>,<DR_RANGE>") {
    _queryable = true;
}

uint32_t CmdTxChannel::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {

        std::vector<uint32_t> channels = CommandTerminal::Dot()->getChannels();
        std::vector<uint8_t> ranges = CommandTerminal::Dot()->getChannelRanges();
        std::vector<uint16_t> mask = CommandTerminal::Dot()->getChannelMask();

        CommandTerminal::Serial()->writef("Index\tFrequency  DR Max Min  On\r\n");

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            for (int8_t i = 0; i < 16; i++) {
                if (channels[i] != 0)
                    CommandTerminal::Serial()->writef("  %d\t%d      %X   %X    %d\r\n", i, channels[i], ranges[i] >> 4, ranges[i] & 0xF, (mask[0] & (0x1 << i)) ? 1 : 0);
                else
                    CommandTerminal::Serial()->writef("  %d\t    %d          %X   %X    0\r\n", i, channels[i], ranges[i] >> 4, ranges[i] & 0xF);
            }

            if (channels[16] != 0)
                CommandTerminal::Serial()->writef("  R2\t%d      %X   %X\r\n", channels[16], ranges[16] >> 4, ranges[16] & 0xF);
            else
                CommandTerminal::Serial()->writef("  R2\t    %d          %X   %X\r\n", channels[16], ranges[16], ranges[168]);

        } else {
            if (CommandTerminal::Dot()->getFrequencySubBand() > 0) {
                uint8_t offset = (CommandTerminal::Dot()->getFrequencySubBand() - 1) * 8;

                for (int i = 0; i < 8; i++) {
                    if ((CommandTerminal::Dot()->getFrequencySubBand() % 2) == 1) {
                        CommandTerminal::Serial()->writef("  %d\t%d      %X   %X    %d\r\n", i, channels[i], ranges[i] >> 4, ranges[i] & 0xF, (mask[(offset + i)/16] & (0x0001 << i)) ? 1 : 0);
                    } else {
                        CommandTerminal::Serial()->writef("  %d\t%d      %X   %X    %d\r\n", i, channels[i], ranges[i] >> 4, ranges[i] & 0xF, (mask[(offset + i)/16] & (0x0001 << (i + 8))) ? 1 : 0);
                    }
                }

                CommandTerminal::Serial()->writef("  U\t%d      %X   %X    %d\r\n", channels[8], ranges[8] >> 4, ranges[8] & 0xF, (mask[4] & (0x0001 << (CommandTerminal::Dot()->getFrequencySubBand() - 1))) ? 1 : 0);
                CommandTerminal::Serial()->writef("  R2\t%d      %X   %X\r\n", channels[9], ranges[9], ranges[9]);
            } else {
                for (size_t i = 0; i < channels.size() - 1; i++) {
                    CommandTerminal::Serial()->writef("  %d\t%d      %X   %X    %d\r\n", i, channels[i], ranges[i] >> 4, ranges[i] & 0xF, (mask[i/16] & (0x0001 << (i%16))) ? 1 : 0);
                }
                uint8_t last = channels.size() - 1;
                CommandTerminal::Serial()->writef("  R2\t%d      %X   %X\r\n", channels[last], ranges[last], ranges[last]);
            }
        }
    }

    if (args.size() == 4) {
        int index;
        int frequency;
        int datarateRange;

        sscanf(args[1].c_str(), "%d", &index);
        sscanf(args[2].c_str(), "%d", &frequency);
        sscanf(args[3].c_str(), "%02x", &datarateRange);

        if (CommandTerminal::Dot()->addChannel(index, frequency, datarateRange) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage("Failed to add channel");
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

        sscanf(args[1].c_str(), "%d", &index);
        sscanf(args[2].c_str(), "%d", &frequency);
        sscanf(args[3].c_str(), "%02x", &datarateRange);

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            if (frequency != 0 && (frequency < int(CommandTerminal::Dot()->getMinFrequency()) || frequency > int(CommandTerminal::Dot()->getMaxFrequency()))) {
                char tmp[256];
                sprintf(tmp, "Invalid frequency, expects (0,%lu-%lu)", CommandTerminal::Dot()->getMinFrequency(), CommandTerminal::Dot()->getMaxFrequency());
                CommandTerminal::setErrorMessage(tmp);
                return false;
            }
        } else {
            CommandTerminal::setErrorMessage("Fixed channel plans cannot be changed, use AT+FSB or AT+CHM to limit");
            return false;
        }

        return true;
    } else {
        CommandTerminal::setErrorMessage("Invalid arguments");
        return false;
    }

}
