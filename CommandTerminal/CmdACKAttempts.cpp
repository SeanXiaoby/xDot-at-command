#include "CmdACKAttempts.h"

CmdACKAttempts::CmdACKAttempts() :
#if MTS_CMD_TERM_VERBOSE
    Command("Require ACK", "AT+ACK", "Enable to require send acknowledgement (0: off, N: number of attempts until ACK received)", "(0-15)")
#else
    Command("AT+ACK")
#endif
{
    _queryable = true;
}

uint32_t CmdACKAttempts::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getAck());
    }
    else if (args.size() == 2)
    {
        int retries;
        sscanf(args[1].c_str(), "%d", &retries);

        if (CommandTerminal::Dot()->setAck(retries) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdACKAttempts::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int retries;
        if (sscanf(args[1].c_str(), "%d", &retries) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (retries < 0 || retries > 15) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid attempts, expects (0-15)");
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
