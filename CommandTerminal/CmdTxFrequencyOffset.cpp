#include "CmdTxFrequencyOffset.h"

CmdTxFrequencyOffset::CmdTxFrequencyOffset() :
    Command("Tx Frequency Offset", "AT+FO",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Transmit frequency offset in Hz",
#else
    "",
#endif
    "(-32768-32768)")
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (offset < -32768 || offset > 32768) {
            CommandTerminal::setErrorMessage("Invalid offset, expects (-32768-32768)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
