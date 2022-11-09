#include "CmdSnr.h"
#include "Lora.h"

static inline void printSnrVal(int16_t val, const char* end) {
    CommandTerminal::Serial()->writef("%d.%d%s", val / 10, abs(val % 10), end);
}

CmdSnr::CmdSnr() :
#if MTS_CMD_TERM_VERBOSE
    Command("Signal To Noise Ratio", "AT+SNR", "Display signal to noise ratio of received packets: last, min, max, avg in dB", "(-20.0-20.0),(-20.0-20.0),(-20.0-20.0),(-20.0-20.0)")
#else
    Command("AT+SNR")
#endif
{
    _queryable = true;
}

uint32_t CmdSnr::action(const std::vector<std::string>& args)
{
    mDot::snr_stats stats = CommandTerminal::Dot()->getSnrStats();
    if (stats.last == lora::INVALID_SNR) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::Serial()->writef("No data\r\n");
#endif
        return 1;
    }
    printSnrVal(stats.last, ", ");
    printSnrVal(stats.min, ", ");
    printSnrVal(stats.max, ", ");
    printSnrVal(stats.avg, "\r\n");


    return 0;
}
