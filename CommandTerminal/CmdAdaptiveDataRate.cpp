#include "CmdAdaptiveDataRate.h"


CmdAdaptiveDataRate::CmdAdaptiveDataRate() :
#if MTS_CMD_TERM_VERBOSE
    Command("Adaptive Data Rate", "AT+ADR", "Enable/disable Adaptive Data Rate (0: off, 1: on), [ADR_ACK_LIMIT (1-127)], [ADR_ACK_DELAY (1-127)]", "(0,1), (1,127), (1,127)")
#else
    Command("AT+ADR")
#endif
{
    _queryable = true;
}

uint32_t CmdAdaptiveDataRate::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d, %d, %d\r\n",
                                          CommandTerminal::Dot()->getAdr(),
                                          CommandTerminal::Dot()->getAdrAckLimit(),
                                          CommandTerminal::Dot()->getAdrAckDelay());
    }
    else if (args.size() > 1)
    {
        bool enable = (args[1] == "1");
        if (CommandTerminal::Dot()->setAdr(enable) != mDot::MDOT_OK)
        {
            return 1;
        }

        if (args.size() > 2)
        {
            int input;
            sscanf(args[2].c_str(), "%d", &input);
            CommandTerminal::Dot()->setAdrAckLimit(input);

            if (args.size() > 3)
            {
                sscanf(args[3].c_str(), "%d", &input);
                CommandTerminal::Dot()->setAdrAckDelay(input);
            }
        }
    }

    return 0;
}

bool CmdAdaptiveDataRate::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args[1] != "1" && args[1] != "0") {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
#endif
        return false;
    }
    
    int input;

    if (args.size() == 2) {
        return true;
    } else {
        sscanf(args[2].c_str(), "%d", &input);

        if (input < 1 || input > 127) {
    #if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects ADR_ACK_LIMIT (1-127)");
    #endif
            return false;
        }
    }

    if (args.size() == 3)
    {
        return true;
    } else {
        sscanf(args[3].c_str(), "%d", &input);
        if (input < 1 || input > 127) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects ADR_ACK_DELAY (1-127)");
#endif
            return false;
        }
    }

    if (args.size() == 4) {
        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
