#include "CmdTxNextSize.h"

CmdTxNextSize::CmdTxNextSize()
: Command("Tx Size", "AT+TXS",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get available payload",
#else
    "",
#endif
    "(0-242)") {
    _queryable = true;
}

uint32_t CmdTxNextSize::action(const std::vector<std::string>& args) {

    CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getNextTxMaxSize());
    return 0;
}

bool CmdTxNextSize::verify(const std::vector<std::string>& args) {

    if (args.size() > 1)
        return false;

    return true;
}
