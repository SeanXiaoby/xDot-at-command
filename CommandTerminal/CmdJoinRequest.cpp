#include "CmdJoinRequest.h"
#include "CommandTerminal.h"

CmdJoinRequest::CmdJoinRequest()
:
  Command("Join Network", "AT+JOIN",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Join network, provide argument of '1' to force join (acquire network address and session keys)",
#else
    "",
#endif
    "(force:1)") {

}

uint32_t CmdJoinRequest::action(const std::vector<std::string>& args) {
    // Detach the serial rx irq handler temporarily here to allow for deep sleep
    //CommandTerminal::Serial()->detach();

    std::string buf;

    if (args.size() > 1 && args[1] == "1")
        CommandTerminal::Dot()->resetNetworkSession();

    if (CommandTerminal::Dot()->getJoinMode() == 0) {
        CommandTerminal::Serial()->writef("Join not necessary for Manual Join Mode\r\n");
        //CommandTerminal::Serial()->attach();
        return 0;
    } else if (CommandTerminal::Dot()->getJoinMode() == 3) {
        CommandTerminal::Serial()->writef("Join not necessary for Peer-to-Peer\r\n");
        //CommandTerminal::Serial()->attach();
        return 0;
    }

    int32_t code = CommandTerminal::Dot()->joinNetworkOnce();

    if (code == mDot::MDOT_OK) {
        CommandTerminal::Serial()->writef("Successfully joined network\r\n");
        //CommandTerminal::Serial()->attach();
        return 0;
    } else {
        std::string error = mDot::getReturnCodeString(code)  + " - " + CommandTerminal::Dot()->getLastError();
        CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
    }

    //CommandTerminal::Serial()->attach();
    return 1;
}

bool CmdJoinRequest::verify(const std::vector<std::string>& args) {
    if (args.size() == 1 || (args.size() == 2 && args[1] == "1"))
        return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
