#include "CmdAttention.h"

CmdAttention::CmdAttention() : Command("Attention", "AT",
#if defined(TARGET_MTS_MDOT_F411RE)
        "Attention",
#else
        "",
#endif
        "NONE")
{
}

uint32_t CmdAttention::action(const std::vector<std::string>& args) {
    return 0;
}

