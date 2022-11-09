#include "CmdDebugBaudRate.h"

CmdDebugBaudRate::CmdDebugBaudRate() :
#if defined(TARGET_MTS_MDOT_F411RE)
        Command("Debug Baud Rate", "AT+DIPR", "Set debug serial baud rate, default: 115200", "(2400,4800,9600,19200,38400,57600,115200,230400,460800,921600)")
#else
        Command("Debug Baud Rate", "AT+DIPR", "", "(2400,4800,9600,19200,38400,57600,115200,230400)")
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
            CommandTerminal::Serial()->writef("Set Debug Baud Rate: %d\r\n", baudrate);
        } else {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
