/*
 * CmdJoinDelay.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdJoinDelay.h"

CmdJoinDelay::CmdJoinDelay() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Delay", "AT+JD", "Number of seconds before receive windows are opened for join (1 - 15)", "(1-15)")
#else
    Command("AT+JD")
#endif
{

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
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid join delay, expects (1-15)");
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
