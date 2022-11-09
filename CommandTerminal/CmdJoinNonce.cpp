#include "CmdJoinNonce.h"

CmdJoinNonce::CmdJoinNonce()
:
  Command("Join Nonces", "AT+JN",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set OTA Join Nonce",
#else
    "",
#endif
    "(0-65535),(0-16777215)") {
    _queryable = true;
}

uint32_t CmdJoinNonce::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u,%u\r\n", CommandTerminal::Dot()->getDevNonce(), CommandTerminal::Dot()->getAppNonce());
    } else if (args.size() == 3) {

        int nonce;
        sscanf(args[1].c_str(), "%d", &nonce);

        if (CommandTerminal::Dot()->setDevNonce(nonce) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }

        sscanf(args[2].c_str(), "%d", &nonce);

        if (CommandTerminal::Dot()->setAppNonce(nonce) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
                CommandTerminal::setErrorMessage("Invalid dev nonce, expects (0-65535)");
                return false;
            }
        }

        if (sscanf(args[2].c_str(), "%d", &nonce) == 1) {
            if (nonce < 0 || nonce > 16777215) {
                CommandTerminal::setErrorMessage("Invalid app nonce, expects (0-16777215)");
                return false;
            }
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

