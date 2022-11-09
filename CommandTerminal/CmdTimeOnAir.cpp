#include "CmdTimeOnAir.h"

CmdTimeOnAir::CmdTimeOnAir() :
#if MTS_CMD_TERM_VERBOSE
    Command("Time on air", "AT+TOA", "Get time in ms of packet tx with current datarate", "(0-242)")
#else
    Command("AT+TOA")
#endif
{
    _queryable = true;
}

uint32_t CmdTimeOnAir::action(const std::vector<std::string>& args) {

    int bytes;

    sscanf(args[1].c_str(), "%d", &bytes);

    CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getTimeOnAir(bytes + 13));

    return 0;
}

bool CmdTimeOnAir::verify(const std::vector<std::string>& args) {

    if (args.size() < 2) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::setErrorMessage("Invalid parameter, expects (0-242)");
#endif
        return false;
    }

    int bytes;

    sscanf(args[1].c_str(), "%d", &bytes);

    if (bytes < 0 || bytes > 242) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::setErrorMessage("Invalid parameter, expects (0-242)");
#endif
        return false;
    }

    return true;
}
