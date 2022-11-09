#include "CmdSendContinuous.h"
#include "CommandTerminal.h"

CmdSendContinuous::CmdSendContinuous() :
#if MTS_CMD_TERM_VERBOSE
    Command("Send Continuous", "AT+SENDC", "Send un-modulated data continuously", "[TIMEOUT],[FREQUENCY],[POWER]")
#else
    Command("AT+SENDC")
#endif
{
}

uint32_t CmdSendContinuous::action(const std::vector<std::string>& args) {

    int timeout = 0;
    int frequency = 0;
    int power = -1;

    if (args.size() > 1) {
        sscanf(args[1].c_str(), "%d", &timeout);
    }
    if (args.size() > 2) {
        sscanf(args[2].c_str(), "%d", &frequency);
    }
    if (args.size() > 3) {
        sscanf(args[3].c_str(), "%d", &power);
    }

    CommandTerminal::Dot()->sendContinuous(true, timeout, frequency, power);
    CommandTerminal::Serial()->clearEscaped();
    return 0;
}

bool CmdSendContinuous::verify(const std::vector<std::string>& args)
{
    int arg;

    if (args.size() == 1)
        return true;

    // Verify all optional arguments are valid numbers
    // No range checking because this command is for test use
    for (size_t i = 1; i < args.size(); ++i) {
        if (sscanf(args[i].c_str(), "%d", &arg) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }
    }

    return true;
}