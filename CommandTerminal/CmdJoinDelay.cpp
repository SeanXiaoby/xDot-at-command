/*
 * CmdJoinDelay.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdJoinDelay.h"

CmdJoinDelay::CmdJoinDelay()
:
  Command("Join Delay", "AT+JD",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Number of seconds before receive windows are opened for join (1 - 15)",
#else
    "",
#endif
    "(1-15)") {

    _queryable = true;
}

CmdJoinDelay::~CmdJoinDelay()
{
    // TODO Auto-generated destructor stub
}

uint32_t CmdJoinDelay::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinDelay());
    } else if (args.size() == 2) {
        uint32_t joinDelay;
        sscanf(args[1].c_str(), "%lu", &joinDelay);

        if (CommandTerminal::Dot()->setJoinDelay(joinDelay) != mDot::MDOT_OK) {

            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }
    return 0;
}

bool CmdJoinDelay::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        int joinDelay;
        if (sscanf(args[1].c_str(), "%d", &joinDelay) == 1) {
            if (joinDelay > 15 || joinDelay < 1) {
                CommandTerminal::setErrorMessage("Invalid join delay, expects (1-15)");
                return false;
            }
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
