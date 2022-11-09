#include "CmdRxFrequency.h"

CmdRxFrequency::CmdRxFrequency() :
#if MTS_CMD_TERM_VERBOSE
    Command("Rx Frequency", "AT+RXF", "Set the Rx frequency for +RECV,+RECVC", "SEE PLAN")
#else
    Command("AT+RXF")
#endif
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
        if (CommandTerminal::Dot()->setRxFrequency(frequency) != mDot::MDOT_OK) {
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
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (frequency != 0 && (frequency < int(CommandTerminal::Dot()->getMinFrequency()) || frequency > int(CommandTerminal::Dot()->getMaxFrequency()))) {            
#if MTS_CMD_TERM_VERBOSE
            char tmp[256];
            sprintf(tmp, "Invalid frequency, expects (0,%lu-%lu)", CommandTerminal::Dot()->getMinFrequency(), CommandTerminal::Dot()->getMaxFrequency());
            CommandTerminal::setErrorMessage(tmp);
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
