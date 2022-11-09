#include "CmdChannelMask.h"
#include "ChannelPlan.h"

CmdChannelMask::CmdChannelMask()
:
  Command("Channel Mask", "AT+CHM",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get/set channel mask (OFFSET:0-4,MASK:0000-FFFF)",
#else
        "",
#endif
   "(OFFSET:0-4,MASK:0000-FFFF)")
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (lora::ChannelPlan::IsPlanDynamic(CommandTerminal::Dot()->getFrequencyBand())) {
            if (offset > 0) {
                CommandTerminal::setErrorMessage("Invalid offset, expects (0)");
                return false;
            }
        } else {
            if (offset < 0 || offset > 4) {
                CommandTerminal::setErrorMessage("Invalid offset, expects (0-4)");
                return false;
            }
        }

        if (!isHexString(args[2], 2)) {
            CommandTerminal::setErrorMessage("Invalid mask, expect (0000-FFFF)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
