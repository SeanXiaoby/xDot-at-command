/*
 * CmdAppPort.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdAppPort.h"

CmdAppPort::CmdAppPort()
:
  Command("App Port", "AT+AP",
#if defined(TARGET_MTS_MDOT_F411RE)
        "Port used for application data (1 - 223)",
#else
        "",
#endif
    "(1-223)")
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

            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
                CommandTerminal::setErrorMessage("Invalid app port, expects (1-223)");
                return false;
            }
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
