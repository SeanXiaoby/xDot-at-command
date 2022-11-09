#include "CmdFrequencyBand.h"

CmdFrequencyBand::CmdFrequencyBand() :
        Command("Current Frequency Band", "AT+FREQ",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Current Frequency Band of Device 'US915', 'AU915', 'EU868', 'AS923', 'KR920', 'AS923-JAPAN', 'IN865', or 'RU864'",
#else
    "",
#endif
    "")
{
    _queryable = true;
}

uint32_t CmdFrequencyBand::action(const std::vector<std::string>& args)
                                  {
    if (args.size() == 1)
    {
        // using getChannelPlanName here instead of mDot::FrequencyBandStr allows AT firmware to properly display custom channel plan names
        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->getChannelPlanName().c_str());
    }

    return 0;
}

bool CmdFrequencyBand::verify(const std::vector<std::string>& args)
                              {
    if (args.size() == 1)
        return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
