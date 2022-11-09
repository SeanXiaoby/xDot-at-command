#include "CmdWakeMode.h"

CmdWakeMode::CmdWakeMode() :
#if defined(TARGET_MTS_MDOT_F411RE)
 Command("Wake Mode", "AT+WM", "Wakeup mode, INTERRUPT uses DIO7 as wake-up pin (0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)", "(0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)")
#else
 Command("Wake Mode", "AT+WM", "", "(0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)")
#endif /* TARGET_MTS_MDOT_F411RE */
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (mode != 0 && mode != 1 && mode != 2) {
            CommandTerminal::setErrorMessage("Invalid mode, expects (0:INTERVAL,1:INTERRUPT,2:INTERVAL_OR_INTERRUPT)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
