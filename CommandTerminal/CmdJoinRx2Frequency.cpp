#include "CmdJoinRx2Frequency.h"
#include "CommandTerminal.h"

CmdJoinRx2Frequency::CmdJoinRx2Frequency() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Rx2 Frequency", "AT+JR2F", "Join Rx2 frequency", "(US:902000000-928000000,AU:915000000-928000000,EU:863000000-870000000)")
#else
    Command("AT+JR2F")
#endif
{

}

uint32_t CmdJoinRx2Frequency::action(const std::vector<std::string>& args) {

    if (args.size() == 1)
        {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getJoinRx2Frequency());
    }
    else if (args.size() == 2)
             {
        int frequency = 0;

        sscanf(args[1].c_str(), "%d", &frequency);
        if (CommandTerminal::Dot()->setJoinRx2Frequency(frequency) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdJoinRx2Frequency::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int frequency = ULONG_MAX;
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
