#include "CmdDutyCycle.h"


static inline void printDutyBand(uint8_t i) {
    const lora::DutyBand* band = CommandTerminal::Dot()->getDutyBand(i);
    if (band) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::Serial()->writef("%-5" PRIu16 "\t%9" PRIu32 "\t%9" PRIu32 "\t%" PRIu16 "\r\n", 
            (uint16_t)i, band->FrequencyMin, band->FrequencyMax, band->DutyCycle);
#else
        CommandTerminal::Serial()->writef("%" PRIu16 ",%" PRIu32 ",%" PRIu32 ",%" PRIu16 "\r\n", 
            (uint16_t)i, band->FrequencyMin, band->FrequencyMax, band->DutyCycle);
#endif
    }
}

CmdDutyCycle::CmdDutyCycle() :
#if MTS_CMD_TERM_VERBOSE
    Command("Duty Cycle", "AT+DUTY", "Set duty cycle maximum or per band.", "<0-15> or <BAND_INDEX>,<OFF_RATIO>")
#else
    Command("AT+DUTY")
#endif
{
    _queryable = true;
}

uint32_t CmdDutyCycle::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::Serial()->writef( "Max %" PRIu16 "\r\n", (uint16_t)CommandTerminal::Dot()->getDutyCycle());
        CommandTerminal::Serial()->writef( "Index\tFreq Low \tFreq High\tOff Ratio\r\n");
#else
        CommandTerminal::Serial()->writef( "%" PRIu16 "\r\n", (uint16_t)CommandTerminal::Dot()->getDutyCycle());
#endif

        for (uint8_t i = 0; i < CommandTerminal::Dot()->getDutyBands(); ++i) {
            printDutyBand(i);
        }

    } else if (args.size() == 2) {
        int dc;
        sscanf(args[1].c_str(), "%d", &dc);
        if (CommandTerminal::Dot()->setDutyCycle(dc) != mDot::MDOT_OK) {
            return 1;
        }
    } else if (args.size() == 3) {
        int band_index;
        int dc;

        sscanf(args[1].c_str(), "%d", &band_index);
        sscanf(args[2].c_str(), "%d", &dc);
        int32_t ret = CommandTerminal::Dot()->setDutyBandDutyCycle((uint8_t)band_index, dc);
        if (ret == mDot::MDOT_OK) {
            printDutyBand(band_index);
        } else {
            return 1;
        }
    }
    return 0;
}

bool CmdDutyCycle::verify(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        return true;
    } else if (args.size() == 2) {
        int dc;

        sscanf(args[1].c_str(), "%d", &dc);

        if ((dc < 0) || (dc > 15)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0-15)");
#endif
            return false;
        }
        return true;
    } else if (args.size() == 3) {
        int band;
        int dc;

        sscanf(args[1].c_str(), "%d", &band);

        if ((band < 0) || (band >= CommandTerminal::Dot()->getDutyBands())) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects valid duty band index");
#endif
            return false;
        }

        sscanf(args[2].c_str(), "%d", &dc);

        if ((dc < 0) || (dc > 65535)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0-65535)");
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
