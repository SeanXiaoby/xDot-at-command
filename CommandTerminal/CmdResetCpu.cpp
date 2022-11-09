#include "CmdResetCpu.h"

CmdResetCpu::CmdResetCpu() : Command("Reset CPU", "ATZ",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Reset the CPU",
#else
    "",
#endif
    "NONE")
{
}

uint32_t CmdResetCpu::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("\r\nOK\r\n");
    wait(0.5);
    CommandTerminal::Dot()->resetCpu();
    return 0;
}

