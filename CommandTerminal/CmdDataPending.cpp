#include "CmdDataPending.h"

CmdDataPending::CmdDataPending() :
        Command("Data Pending", "AT+DP",
#if defined(TARGET_MTS_MDOT_F411RE)
      "Indicator of data in queue on server",
#else
    "",
#endif
    "(0,1)")
{
    _queryable = true;
}

uint32_t CmdDataPending::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getDataPending());
    return 0;
}

