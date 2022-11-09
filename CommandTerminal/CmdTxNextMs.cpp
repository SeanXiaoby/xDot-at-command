#include "CmdTxNextMs.h"

CmdTxNextMs::CmdTxNextMs() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Next", "AT+TXN", "Get time in ms until next free channel", "(0-2793000)")
#else
    Command("AT+TXN")
#endif
{
    _queryable = true;
}

uint32_t CmdTxNextMs::action(const std::vector<std::string>& args) {

    CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getNextTxMs());
    return 0;
}