#include "CmdPingPeriod.h"

CmdPingPeriod::CmdPingPeriod() :
#if MTS_CMD_TERM_VERBOSE
    Command("Ping Periodicity", "AT+PP", "Set the periodicity of class B ping slots as number of pings per interval = 2^(7-periodicity)", "(0-7)")
#else
    Command("AT+PP")
#endif
{
    _queryable = true;
}

uint32_t CmdPingPeriod::action(const std::vector<std::string>& args)
{
    mDot* dot = CommandTerminal::Dot();
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", dot->getPingPeriodicity());
    } else if (args.size() == 2) {
        uint32_t periodicity;
        sscanf(args[1].c_str(), "%lu", &periodicity);

        if (dot->setPingPeriodicity(static_cast<uint8_t>(periodicity)) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdPingPeriod::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        uint32_t periodicity;
        if (sscanf(args[1].c_str(), "%lu", &periodicity) == 1) {
            if (periodicity > 7) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid periodicity, expects (0-7)");
#endif
                return false;
            }

            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
