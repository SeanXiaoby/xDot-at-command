#include "CmdDefaultFrequencyBand.h"
#include "ChannelPlans.h"

CmdDefaultFrequencyBand::CmdDefaultFrequencyBand() :
Command("Default Frequency Band", "AT+DFREQ",
#if defined(DEBUG_MAC)
    "Frequency Band Device Was Manufactured For 'US915', 'AU915', 'EU868', 'AS923', 'KR920', 'AS923-JAPAN', 'IN865', 'RU864', or 'NONE'",
#else
    "",
#endif
    "(NONE,US915,AU915,EU868,AS923,KR920,AS923-JAPAN,IN865,RU864)")
{
    _queryable = true;
}

uint32_t CmdDefaultFrequencyBand::action(const std::vector<std::string>& args)
                                  {
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->FrequencyBandStr(CommandTerminal::Dot()->getDefaultFrequencyBand()).c_str());
    }
    else if (args.size() == 2)
    {
        uint8_t band = lora::ChannelPlan::US915;
        std::string band_str = mts::Text::toUpper(args[1]);

        if (mDot::FrequencyBandStr(lora::ChannelPlan::NONE) == band_str) {
            band = lora::ChannelPlan::NONE;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::US915) == band_str) {
            band = lora::ChannelPlan::US915_OLD;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::AU915) == band_str) {
            band = lora::ChannelPlan::AU915_OLD;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::EU868) == band_str) {
            band = lora::ChannelPlan::EU868_OLD;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::AS923) == band_str) {
            band = lora::ChannelPlan::AS923;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::KR920) == band_str) {
            band = lora::ChannelPlan::KR920;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::AS923_JAPAN) == band_str) {
            band = lora::ChannelPlan::AS923_JAPAN;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::IN865) == band_str) {
            band = lora::ChannelPlan::IN865;
        }
        if (mDot::FrequencyBandStr(lora::ChannelPlan::RU864) == band_str) {
            band = lora::ChannelPlan::RU864;
        }

        if (CommandTerminal::Dot()->setDefaultFrequencyBand(band) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            return 1;
        }
    }

    return 0;
}

bool CmdDefaultFrequencyBand::verify(const std::vector<std::string>& args)
                              {
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        std::string band = mts::Text::toUpper(args[1]);

        if (mDot::FrequencyBandStr(lora::ChannelPlan::NONE) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::US915) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::AU915) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::EU868) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::AS923) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::KR920) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::AS923_JAPAN) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::IN865) != band &&
            mDot::FrequencyBandStr(lora::ChannelPlan::RU864) != band)
        {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (NONE,US915,AU915,EU868,AS923,KR920,AS923-JAPAN,IN865,RU864)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
