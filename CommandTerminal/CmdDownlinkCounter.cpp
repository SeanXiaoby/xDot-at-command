#include "CmdDownlinkCounter.h"

CmdDownlinkCounter::CmdDownlinkCounter() :
#if MTS_CMD_TERM_VERBOSE
    Command("Downlink Counter", "AT+DLC", "Get or set the downlink counter", "(0-4294967295) or (1-8),(0-4294967295)")
#else
    Command("AT+DLC")
#endif
{

    _queryable = true;
}

uint32_t CmdDownlinkCounter::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getDownLinkCounter());
    } else if (args.size() == 2) {
        uint32_t count;
        sscanf(args[1].c_str(), "%lu", &count);

        if (CommandTerminal::Dot()->setDownLinkCounter(count) != mDot::MDOT_OK) {
            return 1;
        }
    } else if (args.size() == 3) {
        uint32_t index;
        sscanf(args[1].c_str(), "%lu", &index);
        if (args[2] == "?") {
            CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getMulticastDownlinkCounter(index-1));
        } else {
            uint32_t count;
            sscanf(args[2].c_str(), "%lu", &count);

            if (CommandTerminal::Dot()->setMulticastDownlinkCounter(index-1, count) != mDot::MDOT_OK) {
                return 1;
            }
        }
    }
    return 0;
}

bool CmdDownlinkCounter::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        uint32_t count;
        if (sscanf(args[1].c_str(), "%lu", &count) == 1) {
            return true;
        }
    }

    if (args.size() == 3) {
        bool ok = true;
        uint32_t count;
        if (sscanf(args[1].c_str(), "%lu", &count) == 1) {
            if (count < 1 || count > 8) {
                ok = false;
            }
        }
        if ((ok && args[2] == "?") || sscanf(args[2].c_str(), "%lu", &count) == 1) {
            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

