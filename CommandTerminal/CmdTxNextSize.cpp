#include "CmdTxNextSize.h"

CmdTxNextSize::CmdTxNextSize() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Size", "AT+TXS", "Get available payload", "(0-242)")
#else
    Command("AT+TXS")
#endif
{
    _queryable = true;
}

uint32_t CmdTxNextSize::action(const std::vector<std::string>& args) {

    CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getNextTxMaxSize());
    return 0;
}
