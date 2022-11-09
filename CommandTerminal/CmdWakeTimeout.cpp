#include "CmdWakeTimeout.h"

CmdWakeTimeout::CmdWakeTimeout() :
 Command("Wake Timeout", "AT+WTO",
 #if defined(TARGET_MTS_MDOT_F411RE)
    "Read serial data until timeout (milliseconds)",
#else
    "",
#endif
    "(0-65000) ms")
{
    _queryable = true;
}

uint32_t CmdWakeTimeout::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeTimeout());
    }
    else if (args.size() == 2)
    {
        int timeout;
        sscanf(args[1].c_str(), "%d", &timeout);

        if (CommandTerminal::Dot()->setWakeTimeout(timeout) != mDot::MDOT_OK) {

            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdWakeTimeout::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int timeout;
        if (sscanf(args[1].c_str(), "%d", &timeout) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (timeout < 0 || timeout > 65000) {
            CommandTerminal::setErrorMessage("Invalid timeout, expects (0-65000) ms");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
