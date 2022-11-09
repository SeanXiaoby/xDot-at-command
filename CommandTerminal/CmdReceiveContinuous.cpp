#include "CmdReceiveContinuous.h"
#include "CommandTerminal.h"

CmdReceiveContinuous::CmdReceiveContinuous() :
#if MTS_CMD_TERM_VERBOSE
    Command("Receive Continuous", "AT+RECVC", "Continuously receive and display packets. (escape sequence: +++)", "out: (string:242) or (hex:242)")
#else
    Command("AT+RECVC")
#endif
{
}

uint32_t CmdReceiveContinuous::action(const std::vector<std::string>& args) {
    CommandTerminal::Serial()->clearEscaped();
    while (true) {
        CommandTerminal::Dot()->openRxWindow(0);

        osDelay(1000);

        printRecvData();

        if (CommandTerminal::Serial()->escaped()) {
            CommandTerminal::Dot()->closeRxWindow();
            CommandTerminal::Serial()->clearEscaped();
            return 0;
        }
    }
}
