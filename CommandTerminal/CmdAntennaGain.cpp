#include "CmdAntennaGain.h"

CmdAntennaGain::CmdAntennaGain() : Command("Antenna Gain", "AT+ANT",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Gain in dBi of installed antenna (-128-127)",
#else
    "",
#endif
    "(-128-127)")
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (gain < -128 || gain > 127) {
            CommandTerminal::setErrorMessage("Invalid gain, expects (-128-127)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
