#include "CmdFrequencySubBand.h"

CmdFrequencySubBand::CmdFrequencySubBand() :
        Command("Frequency Sub-band", "AT+FSB",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the frequency sub-band for US915 and AU915, (0:ALL, 1-8)",
#else
    "",
#endif
    "(0-8)")
{
    _queryable = true;
}

uint32_t CmdFrequencySubBand::action(const std::vector<std::string>& args)
{

    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getFrequencySubBand());
    }
    else if (args.size() == 2)
    {
        uint32_t band;
        sscanf(args[1].c_str(), "%lu", &band);

        if (CommandTerminal::Dot()->setFrequencySubBand(band) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdFrequencySubBand::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int band;
        if (sscanf(args[1].c_str(), "%d", &band) != 1) {
            CommandTerminal::setErrorMessage("Invalid arguments");
            return false;
        }

        if (band < mDot::FSB_ALL || band > mDot::FSB_8) {
            CommandTerminal::setErrorMessage("Invalid channel band, expects (0-8)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
