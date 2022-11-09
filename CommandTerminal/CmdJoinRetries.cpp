#include "CmdJoinRetries.h"

CmdJoinRetries::CmdJoinRetries()
:
  Command("Join Retries", "AT+JR",
#if defined(TARGET_MTS_MDOT_F411RE)
    "US915/AU915 AUTO_OTA Frequency sub-band search retries (0:disable,1-255:attempts)",
#else
    "",
#endif
    "(0-255)") {
    _queryable = true;
}

uint32_t CmdJoinRetries::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinRetries());
    } else if (args.size() == 2) {

        int retries;
        sscanf(args[1].c_str(), "%d", &retries);

        if (CommandTerminal::Dot()->setJoinRetries(retries) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
                CommandTerminal::setErrorMessage("Invalid retries, expects (0-255)");
                return false;
            }
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

