#include "CmdClassBTimeout.h"

CmdClassBTimeout::CmdClassBTimeout() :
#if MTS_CMD_TERM_VERBOSE
    Command("Class B Timeout", "AT+BTO", "Set the timeout the network expects an ACK sent for a confirmed downlink received in a Class B window", "(0-120)")
#else
    Command("AT+BTO")
#endif
{
    _queryable = true;
}

uint32_t CmdClassBTimeout::action(const std::vector<std::string>& args)
{
    mDot* dot = CommandTerminal::Dot();
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", dot->getClassBTimeout());
    } else if (args.size() == 2) {
        uint32_t timeout;
        sscanf(args[1].c_str(), "%lu", &timeout);

        if (dot->setClassBTimeout(static_cast<uint8_t>(timeout)) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdClassBTimeout::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        uint32_t timeout;
        if (sscanf(args[1].c_str(), "%lu", &timeout) == 1) {
            if (timeout > 120) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid timeout, expects (0-120)");
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
