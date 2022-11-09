#include "CmdTxWait.h"

CmdTxWait::CmdTxWait() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Wait", "AT+TXW", "Enable/disable waiting for rx windows to expire after send. (0: off, 1: on)", "(0,1)")
#else
    Command("AT+TXW")
#endif
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
