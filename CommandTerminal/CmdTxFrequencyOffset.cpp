#include "CmdTxFrequencyOffset.h"

CmdTxFrequencyOffset::CmdTxFrequencyOffset() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Frequency Offset", "AT+FO", "Transmit frequency offset in Hz", "(-32768-32768)")
#else
    Command("AT+FO")
#endif
{
    _queryable = true;
}

uint32_t CmdTxFrequencyOffset::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getFrequencyOffset());
    }
    else if (args.size() == 2)
    {
        int offset;
        sscanf(args[1].c_str(), "%d", &offset);

        CommandTerminal::Dot()->setFrequencyOffset(offset);
    }

    return 0;
}

bool CmdTxFrequencyOffset::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int offset;
        if (sscanf(args[1].c_str(), "%d", &offset) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (offset < -32768 || offset > 32768) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid offset, expects (-32768-32768)");
#endif
            return false;
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
