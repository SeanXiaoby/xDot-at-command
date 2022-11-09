#include "CmdRtc.h"

CmdRtc::CmdRtc() :
#if MTS_CMD_TERM_VERBOSE
    Command("Real Time Clock", "AT+RTC", "Set real time clock to number of seconds since January 1, 1970 (the UNIX timestamp)", "(0-4294967295)")
#else
    Command("AT+RTC")
#endif
{
    _queryable = true;
}

uint32_t CmdRtc::action(const std::vector<std::string>& args)
{
    uint32_t value;
    if (args.size() == 1)
    {
        value = time(NULL);
        CommandTerminal::Serial()->writef("%" PRIu32 "\r\n", value);
    }
    else if (args.size() == 2)
    {

        if (sscanf(args[1].c_str(), "%" PRIu32, &value) == 1)
        {
            time_t new_time = value;
            set_time(new_time);
#if MTS_CMD_TERM_VERBOSE
            value = time(NULL);
            CommandTerminal::Serial()->writef("Set RTC : %" PRIu32 "\r\n", value);
#endif
        } else {
            return 1;
        }
    }

    return 0;
}

bool CmdRtc::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        uint32_t value;

        if (sscanf(args[1].c_str(), "%" PRIu32 , &value) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
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
