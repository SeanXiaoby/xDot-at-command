#include "CmdJoinNonceValidation.h"

CmdJoinNonceValidation::CmdJoinNonceValidation() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Nonce Validation", "AT+JNV", "Enable/disable join nonce validation", "(0,1)")
#else
    Command("AT+JNV")
#endif
{
    _queryable = true;
}

uint32_t CmdJoinNonceValidation::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinNonceValidation());
    } else if (args.size() == 2) {

        int retries;
        sscanf(args[1].c_str(), "%d", &retries);

        if (CommandTerminal::Dot()->setJoinNonceValidation(retries) != mDot::MDOT_OK) {
            return 1;
        }
    }
    return 0;
}

bool CmdJoinNonceValidation::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        int retries;
        if (sscanf(args[1].c_str(), "%d", &retries) == 1) {
            if (retries < 0 || retries > 1) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
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

