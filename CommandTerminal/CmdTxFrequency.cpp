#include "CmdTxFrequency.h"
#include <climits>


CmdTxFrequency::CmdTxFrequency() :
    Command("Tx Frequency", "AT+TXF",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set Tx frequency",
#else
    "",
#endif
    "SEE PLAN") {
    _queryable = true;
}

uint32_t CmdTxFrequency::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getTxFrequency());
    }
    else if (args.size() == 2)
    {
        int frequency = 0;

        sscanf(args[1].c_str(), "%d", &frequency);
        if (CommandTerminal::Dot()->setTxFrequency(frequency) != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdTxFrequency::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int frequency = ULONG_MAX;
        if (sscanf(args[1].c_str(), "%d", &frequency) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (frequency != 0 && (frequency < int(CommandTerminal::Dot()->getMinFrequency()) || frequency > int(CommandTerminal::Dot()->getMaxFrequency()))) {
            char tmp[256];
            sprintf(tmp, "Invalid frequency, expects (0,%lu-%lu)", CommandTerminal::Dot()->getMinFrequency(), CommandTerminal::Dot()->getMaxFrequency());
            CommandTerminal::setErrorMessage(tmp);
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

