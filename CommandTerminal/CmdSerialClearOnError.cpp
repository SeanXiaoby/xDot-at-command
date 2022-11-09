#include "CmdSerialClearOnError.h"

CmdSerialClearOnError::CmdSerialClearOnError() :
        Command("Clear On Error", "AT+SDCE",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Serial clear on error if enabled data that cannot be sent will be discarded",
#else
    "",
#endif
    "(0:off,1:on)")
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
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
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
