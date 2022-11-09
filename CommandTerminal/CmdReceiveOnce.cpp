#include "CmdReceiveOnce.h"
#include "CommandTerminal.h"

CmdReceiveOnce::CmdReceiveOnce() :
#if MTS_CMD_TERM_VERBOSE
    Command("Receive Once", "AT+RECV", "Receive and display one packet.", "(string:242) or (hex:242)")
#else
    Command("AT+RECV")
#endif
{}

uint32_t CmdReceiveOnce::action(const std::vector<std::string>& args) {
    printRecvData();
    return 0;
}
