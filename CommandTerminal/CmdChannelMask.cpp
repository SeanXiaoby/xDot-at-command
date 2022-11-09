#include "CmdChannelMask.h"
#include "ChannelPlan.h"

CmdChannelMask::CmdChannelMask() :
#if MTS_CMD_TERM_VERBOSE
    Command("Channel Mask", "AT+CHM", "Get/set channel mask (OFFSET:0-4,MASK:0000-FFFF)", "(OFFSET:0-4,MASK:0000-FFFF)")
#else
    Command("AT+CHM")
#endif
{
    _queryable = true;
}

uint32_t CmdChannelMask::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        if (CommandTerminal::Dot()->getVerbose())
            CommandTerminal::Serial()->writef("Channel Mask: ");

        std::vector<uint16_t> mask = CommandTerminal::Dot()->getChannelMask();

        for (int i = int(mask.size()) - 1; i >= 0; i--) {
            CommandTerminal::Serial()->writef("%04X", mask[i]);
        }

        CommandTerminal::Serial()->writef("\r\n");

    } else if (args.size() == 3) {
        int temp = 0;
        int offset = 0;
        uint16_t mask = 0;

        sscanf(args[1].c_str(), "%d", &offset);

        // Convert the ASCII hex data to binary...
        sscanf(&args[2][0], "%02x", &temp);
        mask = uint8_t(temp) << 8;
        sscanf(&args[2][2], "%02x", &temp);
        mask |= uint8_t(temp);

        if (CommandTerminal::Dot()->setChannelMask(offset, mask) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdChannelMask::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 3) {

        int offset;
        if (sscanf(args[1].c_str(), "%d", &offset) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            if (offset > 0) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid offset, expects (0)");
#endif
                return false;
            }
        } else {
            if (offset < 0 || offset > 4) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid offset, expects (0-4)");
#endif
                return false;
            }
        }

        if (!isHexString(args[2], 2)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid mask, expect (0000-FFFF)");
#endif
            return false;
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
