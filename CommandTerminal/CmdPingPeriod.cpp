#include "CmdPingPeriod.h"

CmdPingPeriod::CmdPingPeriod() :
    Command("Ping Periodicity", "AT+PP",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the periodicity of class B ping slots as number of pings per interval = 2^(7-periodicity)",
#else
    "",
#endif
    "(0-7)")
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
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
                CommandTerminal::setErrorMessage("Invalid periodicity, expects (0-7)");
                return false;
            }

            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
