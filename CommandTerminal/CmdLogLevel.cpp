#include "CmdLogLevel.h"

CmdLogLevel::CmdLogLevel() :
#if MTS_CMD_TERM_VERBOSE
        Command("Debug Log Level", "AT+LOG", "Enable/disable debug logging. (0: off, 1:Fatal - 6:Trace)", "(0-6)")
#else
        Command("AT+LOG")
#endif
{
    _queryable = true;
}

uint32_t CmdLogLevel::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getLogLevel());
    }
    else if (args.size() == 2)
    {
        int level;
        sscanf(args[1].c_str(), "%d", &level);

        if (CommandTerminal::Dot()->setLogLevel(level) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdLogLevel::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int level;
        if (sscanf(args[1].c_str(), "%d", &level) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (level < 0 || level > 6) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid level, expects (0-6)");
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
