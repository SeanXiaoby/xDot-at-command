#include "CmdTxNextMs.h"

CmdTxNextMs::CmdTxNextMs()
: Command("Tx Next", "AT+TXN",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get time in ms until next free channel",
#else
    "",
#endif
    "(0-2793000)") {
    _queryable = true;
}

uint32_t CmdTxNextMs::action(const std::vector<std::string>& args) {

    CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getNextTxMs());
    return 0;
}

bool CmdTxNextMs::verify(const std::vector<std::string>& args) {

    if (args.size() > 1)
        return false;

    return true;
}
