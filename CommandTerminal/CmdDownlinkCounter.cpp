#include "CmdDownlinkCounter.h"

CmdDownlinkCounter::CmdDownlinkCounter()
:
  Command("Downlink Counter", "AT+DLC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get or set the downlink counter",
#else
    "",
#endif
    "(0-4294967295) or (1-8),(0-4294967295)") {

    _queryable = true;
}

uint32_t CmdDownlinkCounter::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getDownLinkCounter());
    } else if (args.size() == 2) {
        uint32_t count;
        sscanf(args[1].c_str(), "%lu", &count);

        if (CommandTerminal::Dot()->setDownLinkCounter(count) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

