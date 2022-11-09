#include "CmdWakeMode.h"

CmdWakeMode::CmdWakeMode() :
#if MTS_CMD_TERM_VERBOSE
 Command("Wake Mode", "AT+WM", "Wakeup mode, INTERRUPT uses DIO7 as wake-up pin (0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)", "(0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)")
#else
 Command("AT+WM")
#endif
{
    _queryable = true;
}

uint32_t CmdWakeMode::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeMode());
    }
    else if (args.size() == 2)
    {
        int mode;
        sscanf(args[1].c_str(), "%d", &mode);

        if (CommandTerminal::Dot()->setWakeMode(mode) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdWakeMode::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int mode;
        if (sscanf(args[1].c_str(), "%d", &mode) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (mode != 0 && mode != 1 && mode != 2) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid mode, expects (0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)");
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
