#include "CmdTxWait.h"

CmdTxWait::CmdTxWait() :
        Command("Tx Wait", "AT+TXW",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Enable/disable waiting for rx windows to expire after send. (0: off, 1: on)",
#else
    "",
#endif
    "(0,1)")
{
    _queryable = true;
}

uint32_t CmdTxWait::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getTxWait());
    }
    else if (args.size() == 2)
    {
        if (CommandTerminal::Dot()->setTxWait(args[1] == "1") != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdTxWait::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (args[1] != "0" && args[1] != "1") {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
