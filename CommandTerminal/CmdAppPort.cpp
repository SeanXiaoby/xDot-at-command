/*
 * CmdAppPort.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdAppPort.h"

CmdAppPort::CmdAppPort() :
#if MTS_CMD_TERM_VERBOSE
    Command("App Port", "AT+AP", "Port used for application data (1 - 223)", "(1-223)")
#else
    Command("AT+AP")
#endif
{
    _queryable = true;
}

CmdAppPort::~CmdAppPort()
{
    // TODO Auto-generated destructor stub
}

uint32_t CmdAppPort::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getAppPort());
    } else if (args.size() == 2) {

        int appPort;
        sscanf(args[1].c_str(), "%d", &appPort);

        if (CommandTerminal::Dot()->setAppPort(appPort) != mDot::MDOT_OK) {
            return 1;
        }
    }
    return 0;
}

bool CmdAppPort::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        int appPort;
        if (sscanf(args[1].c_str(), "%d", &appPort) == 1) {
            if (appPort > 223 || appPort < 1) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid app port, expects (1-223)");
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
