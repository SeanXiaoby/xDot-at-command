#include "CmdSerialClearOnError.h"

CmdSerialClearOnError::CmdSerialClearOnError() :
#if MTS_CMD_TERM_VERBOSE
    Command("Clear On Error", "AT+SDCE", "Serial clear on error if enabled data that cannot be sent will be discarded", "(0:off,1:on)")
#else
    Command("AT+SDCE")
#endif
{
    _queryable = true;
}

uint32_t CmdSerialClearOnError::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getSerialClearOnError());
    }
    else if (args.size() == 2)
    {
        bool enable = (args[1] == "1");

        if (CommandTerminal::Dot()->setSerialClearOnError(enable) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdSerialClearOnError::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        if (args[1] != "1" && args[1] != "0") {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
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
