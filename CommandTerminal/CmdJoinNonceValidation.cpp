#include "CmdJoinNonceValidation.h"

CmdJoinNonceValidation::CmdJoinNonceValidation()
:
  Command("Join Nonce Validation", "AT+JNV",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Enable/disable join nonce validation",
#else
    "",
#endif
    "(0,1)") {
    _queryable = true;
}

uint32_t CmdJoinNonceValidation::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinNonceValidation());
    } else if (args.size() == 2) {

        int retries;
        sscanf(args[1].c_str(), "%d", &retries);

        if (CommandTerminal::Dot()->setJoinNonceValidation(retries) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
                CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
                return false;
            }
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

