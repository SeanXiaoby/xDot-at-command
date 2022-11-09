#include "CmdTxPower.h"

CmdTxPower::CmdTxPower() :
    Command("Tx Power", "AT+TXP",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the radio TX power before antenna gain in dBm",
#else
    "",
#endif
    "(Depends on Channel Plan)") {
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (power < CommandTerminal::Dot()->getMinTxPower() || power > CommandTerminal::Dot()->getMaxTxPower())
        {
            char buf[8];
            std::string error = "Invalid power, expects ";
            snprintf(buf, sizeof(buf), "(%d-%d)", CommandTerminal::Dot()->getMinTxPower(), CommandTerminal::Dot()->getMaxTxPower());
            error.append(buf);
            CommandTerminal::setErrorMessage(error);
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

