#include "CmdRxFrequency.h"

CmdRxFrequency::CmdRxFrequency()
:
  Command("Rx Frequency", "AT+RXF",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the Rx frequency for +RECV,+RECVC",
#else
    "",
#endif
    "SEE PLAN")
{
    _queryable = true;
}

uint32_t CmdRxFrequency::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getRxFrequency());
    }
    else if (args.size() == 2)
             {

        int frequency = 0;
        sscanf(args[1].c_str(), "%d", &frequency);
        if (CommandTerminal::Dot()->setRxFrequency(frequency) != mDot::MDOT_OK)
            {

            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            ;
            return 1;
        }
    }

    return 0;
}

bool CmdRxFrequency::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
        {
        int frequency;
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
