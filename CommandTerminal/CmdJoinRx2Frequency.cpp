#include "CmdJoinRx2Frequency.h"
#include "CommandTerminal.h"

CmdJoinRx2Frequency::CmdJoinRx2Frequency()
:
  Command("Join Rx2 Frequency", "AT+JR2F",
  #if defined(TARGET_MTS_MDOT_F411RE)
    "Join Rx2 frequency",
#else
    "",
#endif
    "(US:902000000-928000000,AU:915000000-928000000,EU:863000000-870000000)") {

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
        if (CommandTerminal::Dot()->setJoinRx2Frequency(frequency) != mDot::MDOT_OK)
            {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            ;
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
