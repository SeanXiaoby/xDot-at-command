#include "CmdRxOutput.h"

CmdRxOutput::CmdRxOutput() :
        Command("Rx Output", "AT+RXO",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the Rx output type (0:hexadecimal, 1:raw, 2:extended, 3:extended_hex)",
#else
    "",
#endif
    "(0-3)")
{
    _queryable = true;
}

uint32_t CmdRxOutput::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getRxOutput());
    }
    else if (args.size() == 2)
    {
        uint8_t output = 0xFF;

        if(args[1] == "0") output = 0;
        else if(args[1] == "1") output = 1;
        else if(args[1] == "2") output = 2;
        else if(args[1] == "3") output = 3;

        if (CommandTerminal::Dot()->setRxOutput(output) != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdRxOutput::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (!(args[1] == "0" || args[1] == "1" || args[1] == "2" || args[1] == "3")) {
            CommandTerminal::setErrorMessage("Invalid type, expects (0:hexadecimal, 1:raw, 2:extended, 3:extended_hex)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
