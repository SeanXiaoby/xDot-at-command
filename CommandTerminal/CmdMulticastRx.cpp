#include "CmdMulticastRx.h"

CmdMulticastRx::CmdMulticastRx() :
#if MTS_CMD_TERM_VERBOSE
    Command("Muticast Rx Settings", "AT+MCRX", "Multicast Rx Settings", "(1-8),(DR0-DR15),(FREQ),(-1-7:PERIOD,-1:CLASS_C)")
#else
    Command("AT+MCRX")
#endif
{
    _queryable = true;
}

uint32_t CmdMulticastRx::action(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        int index = 0;

        sscanf(args[1].c_str(), "%d", &index);

        index -= 1;
        CommandTerminal::Serial()->writef("DR%d,%lu,%d\r\n", CommandTerminal::Dot()->getMulticastDatarate(index), CommandTerminal::Dot()->getMulticastFrequency(index), CommandTerminal::Dot()->getMulticastPeriodicity(index));
    }
    else if (args.size() == 5) {
        int index = 0;
        int datarate = 0;
        int period = 0;

        sscanf(args[1].c_str(), "%d", &index);
        sscanf(args[4].c_str(), "%d", &period);

        std::string dr = mts::Text::toUpper(args[2]);

        uint8_t i;

        int res = sscanf(dr.c_str(), "%d", &datarate);

        if (res == 0) {
            for (i = 0; i < 24; i++) {
                if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                    datarate = i;
                    break;
                }
            }
        }

        index -= 1;

        if (CommandTerminal::Dot()->setMulticastDatarate(index, datarate) != mDot::MDOT_OK) {
            return 1;
        }

        int frequency = ULONG_MAX;
        sscanf(args[3].c_str(), "%d", &frequency);

        if (CommandTerminal::Dot()->setMulticastFrequency(index, frequency) != mDot::MDOT_OK) {
            return 1;
        }

        if (CommandTerminal::Dot()->setMulticastPeriodicity(index, period) != mDot::MDOT_OK) {
            return 1;
        }

    }

    return 0;
}

bool CmdMulticastRx::verify(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        int value = 0;

        int res = sscanf(args[1].c_str(), "%d", &value);

        if (res == 0 || value < 1 || value > 8) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid index, expects (1-8)");;
#endif
            return false;
        }

        return true;
    }

    if (args.size() == 5) {
        int value = 0;

        int res = sscanf(args[1].c_str(), "%d", &value);

        if (res == 0 || value < 1 || value > 8) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid index, expects (1-8)");;
#endif
            return false;
        }

        res = sscanf(args[4].c_str(), "%d", &value);

        if (res == 0 || value < -1 || value > 7) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid period, expects (-1-7,-1:CLASS_C)");
#endif
            return false;
        }

        int frequency = ULONG_MAX;
        if (sscanf(args[3].c_str(), "%d", &frequency) != 1) {
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

        if (!CommandTerminal::Dot()->validateRx2DataRate(strToDataRate(args[2]))) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("RX data rate invalid");
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
