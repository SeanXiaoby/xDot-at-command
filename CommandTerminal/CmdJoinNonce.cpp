#include "CmdJoinNonce.h"

CmdJoinNonce::CmdJoinNonce() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Nonces", "AT+JN", "Set OTA Join Nonce", "(0-65535),(0-16777215)")
#else
    Command("AT+JN")
#endif
{
    _queryable = true;
}

uint32_t CmdJoinNonce::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u,%u\r\n", CommandTerminal::Dot()->getDevNonce(), CommandTerminal::Dot()->getAppNonce());
    } else if (args.size() == 3) {

        int nonce;
        sscanf(args[1].c_str(), "%d", &nonce);

        if (CommandTerminal::Dot()->setDevNonce(nonce) != mDot::MDOT_OK) {
            return 1;
        }

        sscanf(args[2].c_str(), "%d", &nonce);

        if (CommandTerminal::Dot()->setAppNonce(nonce) != mDot::MDOT_OK) {
            return 1;
        }
    }
    return 0;
}

bool CmdJoinNonce::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 3) {

        int nonce;
        if (sscanf(args[1].c_str(), "%d", &nonce) == 1) {
            if (nonce < 0 || nonce > 65535) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid dev nonce, expects (0-65535)");
#endif
                return false;
            }
        }

        if (sscanf(args[2].c_str(), "%d", &nonce) == 1) {
            if (nonce < 0 || nonce > 16777215) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid app nonce, expects (0-16777215)");
#endif
                return false;
            }
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

