#include "CmdUplinkCounter.h"

CmdUplinkCounter::CmdUplinkCounter() :
#if MTS_CMD_TERM_VERBOSE
    Command("Uplink Counter", "AT+ULC", "Get or set the uplink counter for the next packet", "(0-4294967295)")
#else
    Command("AT+ULC")
#endif
{
    _queryable = true;
}

uint32_t CmdUplinkCounter::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getUpLinkCounter());
    } else if (args.size() == 2) {

        uint32_t count;
        sscanf(args[1].c_str(), "%lu", &count);

        if (CommandTerminal::Dot()->setUpLinkCounter(count) != mDot::MDOT_OK) {
            return 1;
        }
    }
    return 0;
}

bool CmdUplinkCounter::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        uint32_t count;
        if (sscanf(args[1].c_str(), "%lu", &count) == 1) {
            if (count > 4294967295U) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid uplink counter, expects (0-4294967295)");
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

