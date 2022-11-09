#include "CmdDebugBaudRate.h"

CmdDebugBaudRate::CmdDebugBaudRate() :
#if MTS_CMD_TERM_VERBOSE
    Command("Debug Baud Rate", "AT+DIPR", "Set debug serial baud rate, default: 115200", "(2400,4800,9600,19200,38400,57600,115200,230400,460800,921600)")
#else
    Command("AT+DIPR")
#endif
{
    _queryable = true;
}

uint32_t CmdDebugBaudRate::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getDebugBaud());
    }
    else if (args.size() == 2)
    {
        int baudrate = 0;

        sscanf(args[1].c_str(), "%d", &baudrate);

        if (CommandTerminal::Dot()->setDebugBaud(baudrate) == mDot::MDOT_OK) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::Serial()->writef("Set Debug Baud Rate: %d\r\n", baudrate);
#endif
        } else {
            return 1;
        }
    }

    return 0;
}

bool CmdDebugBaudRate::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int baudrate;

        if (sscanf(args[1].c_str(), "%d", &baudrate) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
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
