#include "CmdWakeDelay.h"

CmdWakeDelay::CmdWakeDelay() :
        Command("Wake Delay", "AT+WD",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Time to wait for data after wakeup signal  (milliseconds)",
#else
    "",
#endif
    "(2-2147483646) ms")
{
    _queryable = true;
}

uint32_t CmdWakeDelay::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeDelay());
    }
    else if (args.size() == 2)
    {
        int delay;
        sscanf(args[1].c_str(), "%d", &delay);

        if (CommandTerminal::Dot()->setWakeDelay(delay) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdWakeDelay::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int delay;
        if (sscanf(args[1].c_str(), "%d", &delay) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (delay < 2 || delay > INT_MAX-1) {
            CommandTerminal::setErrorMessage("Invalid delay, expects (2-2147483646) ms");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
