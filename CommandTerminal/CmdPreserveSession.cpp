/*
 * CmdPreserveSession.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#include "CmdPreserveSession.h"

CmdPreserveSession::CmdPreserveSession()
:
  Command("Preserve Session", "AT+PS",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Save network session info through reset or power down in AUTO_OTA mode (0:off, 1:on)",
#else
    "",
#endif
    "(0,1)")
{
    _queryable = true;
}

CmdPreserveSession::~CmdPreserveSession()
{
    // TODO Auto-generated destructor stub
}

uint32_t CmdPreserveSession::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getPreserveSession());
    } else {
        CommandTerminal::Dot()->setPreserveSession(args[1] == "1");
    }

    return mDot::MDOT_OK;
}

bool CmdPreserveSession::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        if (args[1] != "1" && args[1] != "0") {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
