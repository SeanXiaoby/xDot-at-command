#include "CmdJoinRetries.h"

CmdJoinRetries::CmdJoinRetries() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Retries", "AT+JR", "US915/AU915 AUTO_OTA Frequency sub-band search retries (0:disable,1-255:attempts)", "(0-255)")
#else
    Command("AT+JR")
#endif
{
    _queryable = true;
}

uint32_t CmdJoinRetries::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinRetries());
    } else if (args.size() == 2) {

        int retries;
        sscanf(args[1].c_str(), "%d", &retries);

        if (CommandTerminal::Dot()->setJoinRetries(retries) != mDot::MDOT_OK) {
            return 1;
        }
    }
    return 0;
}

bool CmdJoinRetries::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        int retries;
        if (sscanf(args[1].c_str(), "%d", &retries) == 1) {
            if (retries < 0 || retries > 255) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid retries, expects (0-255)");
#endif
                return false;
            }
            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

