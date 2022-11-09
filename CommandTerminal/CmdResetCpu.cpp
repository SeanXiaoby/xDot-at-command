#include "CmdResetCpu.h"

CmdResetCpu::CmdResetCpu() :
#if MTS_CMD_TERM_VERBOSE
    Command("Reset CPU", "ATZ", "Reset the CPU", "NONE")
#else
    Command("ATZ")
#endif
{
}

uint32_t CmdResetCpu::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("\r\nOK\r\n");
    ThisThread::sleep_for(500ms);
    CommandTerminal::Dot()->resetCpu();
    return 0;
}

