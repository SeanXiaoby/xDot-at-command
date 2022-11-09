#include "CmdAntennaGain.h"

CmdAntennaGain::CmdAntennaGain() :
#if MTS_CMD_TERM_VERBOSE
    Command("Antenna Gain", "AT+ANT", "Gain in dBi of installed antenna (-128-127)", "(-128-127)")
#else
    Command("AT+ANT")
#endif
{
    _queryable = true;
}

uint32_t CmdAntennaGain::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getAntennaGain());
    }
    else if (args.size() == 2)
    {

        int gain;
        sscanf(args[1].c_str(), "%d", &gain);

        if (CommandTerminal::Dot()->setAntennaGain(gain) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdAntennaGain::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int gain;
        if (sscanf(args[1].c_str(), "%d", &gain) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (gain < -128 || gain > 127) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid gain, expects (-128-127)");
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
