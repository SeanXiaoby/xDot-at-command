#include "CmdFrequencySubBand.h"

CmdFrequencySubBand::CmdFrequencySubBand() :
#if MTS_CMD_TERM_VERBOSE
    Command("Frequency Sub-band", "AT+FSB", "Set the frequency sub-band for US915 and AU915, (0:ALL, 1-8)", "(0-8)")
#else
    Command("AT+FSB")
#endif
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
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid arguments");
#endif
            return false;
        }

        if (band < mDot::FSB_ALL || band > mDot::FSB_8) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid channel band, expects (0-8)");
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
