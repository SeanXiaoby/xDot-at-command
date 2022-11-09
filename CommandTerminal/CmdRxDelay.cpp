/*
 * CmdRxDelay.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdRxDelay.h"

CmdRxDelay::CmdRxDelay()
:
  Command("Rx Delay", "AT+RXD",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Number of seconds before receive windows are opened (1 - 15)",
#else
    "",
#endif
    "(1-15)")
{
    _queryable = true;
}

CmdRxDelay::~CmdRxDelay()
{
    // TODO Auto-generated destructor stub
}

uint32_t CmdRxDelay::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getRxDelay());
    } else if (args.size() == 2) {

        int rxDelay;
        sscanf(args[1].c_str(), "%d", &rxDelay);

        if (CommandTerminal::Dot()->setRxDelay(rxDelay) != mDot::MDOT_OK) {

            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }
    return 0;
}

bool CmdRxDelay::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        int rxDelay;
        if (sscanf(args[1].c_str(), "%d", &rxDelay) == 1) {

            if (rxDelay > 15 || rxDelay < 1) {
                CommandTerminal::setErrorMessage("Invalid rx delay, expects (1-15)");
                return false;
            }

            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
