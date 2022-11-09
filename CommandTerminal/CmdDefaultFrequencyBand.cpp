#include "CmdDefaultFrequencyBand.h"


static const uint8_t PLAN_COUNT = 12;
static const lora::ChannelPlan::Plan PLANS[PLAN_COUNT] = {
        lora::ChannelPlan::NONE,        lora::ChannelPlan::US915,   lora::ChannelPlan::AU915,
        lora::ChannelPlan::EU868,       lora::ChannelPlan::AS923,   lora::ChannelPlan::KR920,
        lora::ChannelPlan::AS923_JAPAN, lora::ChannelPlan::IN865,   lora::ChannelPlan::RU864,
        lora::ChannelPlan::AS923_2, 	lora::ChannelPlan::AS923_3, lora::ChannelPlan::AS923_4
        // , lora::ChannelPlan::AS923_JAPAN1, lora::ChannelPlan::AS923_JAPAN2
    };

CmdDefaultFrequencyBand::CmdDefaultFrequencyBand() :
#if MTS_CMD_TERM_VERBOSE
    Command("Default Frequency Band", "AT+DFREQ", "Frequency Band Device Was Manufactured For 'US915', 'AU915', 'EU868', 'AS923', 'AS923-2', 'AS923-3', 'AS923-4', 'KR920', 'AS923-JAPAN', 'IN865', 'RU864', or 'NONE'", "(NONE,US915,AU915,EU868,AS923,AS923-2,AS923-3,AS923-4,KR920,AS923-JAPAN,IN865,RU864)")
#else
    Command("AT+DFREQ")
#endif
{
    _queryable = true;
}

uint32_t CmdDefaultFrequencyBand::action(const std::vector<std::string>& args) {
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->FrequencyBandStr(CommandTerminal::Dot()->getDefaultFrequencyBand()).c_str());
    }
    else if (args.size() == 2)
    {
        uint8_t band = lora::ChannelPlan::US915;
        std::string band_str = mts::Text::toUpper(args[1]);
        std::string exp_band_str;

        for (int i = 0; i < PLAN_COUNT; ++i) {
            if (mDot::FrequencyBandStr(PLANS[i]) == band_str) {
                band = PLANS[i];
            }
        }

        if (CommandTerminal::Dot()->setDefaultFrequencyBand(band) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdDefaultFrequencyBand::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        std::string band = mts::Text::toUpper(args[1]);

        for (int i = 0; i < PLAN_COUNT; ++i) {
            if (mDot::FrequencyBandStr(PLANS[i]) == band) {
                return true;
            }
        }

#if MTS_CMD_TERM_VERBOSE
        // CommandTerminal::setErrorMessage("Invalid parameter, expects (NONE,US915,AU915,EU868,AS923,AS923-2,AS923-3,KR920,AS923-JAPAN,AS923-JAPAN1,AS923-JAPAN2,IN865,RU864)");
        CommandTerminal::setErrorMessage("Invalid parameter, expects (NONE,US915,AU915,EU868,AS923,AS923-2,AS923-3,KR920,AS923-JAPAN,IN865,RU864)");
#endif
        return false;

    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
