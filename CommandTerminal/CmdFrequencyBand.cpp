#include "CmdFrequencyBand.h"

CmdFrequencyBand::CmdFrequencyBand() :
#if MTS_CMD_TERM_VERBOSE
    Command("Current Frequency Band", "AT+FREQ", "Current Frequency Band of Device 'US915', 'AU915', 'EU868', 'AS923', 'AS923-2', 'AS923-3', 'AS923-4', 'KR920', 'AS923-JAPAN', 'IN865', or 'RU864'", "")
#else
    Command("AT+FREQ")
#endif
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
