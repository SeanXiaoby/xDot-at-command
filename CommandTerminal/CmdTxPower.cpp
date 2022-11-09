#include "CmdTxPower.h"

CmdTxPower::CmdTxPower() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Power", "AT+TXP", "Set the radio TX power before antenna gain in dBm", "(Depends on Channel Plan)")
#else
    Command("AT+TXP")
#endif
{
    _queryable = true;
}

uint32_t CmdTxPower::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getTxPower());
    }
    else if (args.size() == 2)
    {
        int power = 0;
        sscanf(args[1].c_str(), "%d", &power);

        if (CommandTerminal::Dot()->setTxPower(power) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdTxPower::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int power = 0;
        if (sscanf(args[1].c_str(), "%d", &power) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (power < CommandTerminal::Dot()->getMinTxPower() || power > CommandTerminal::Dot()->getMaxTxPower())
        {
#if MTS_CMD_TERM_VERBOSE
            char buf[8];
            std::string error = "Invalid power, expects ";
            snprintf(buf, sizeof(buf), "(%d-%d)", CommandTerminal::Dot()->getMinTxPower(), CommandTerminal::Dot()->getMaxTxPower());
            error.append(buf);
            CommandTerminal::setErrorMessage(error);
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

