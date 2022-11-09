#include "CmdDisableDutyCycle.h"

CmdDisableDutyCycle::CmdDisableDutyCycle() :
#if MTS_CMD_TERM_VERBOSE
    Command("Disable Duty Cycle", "AT+DD", "Disable duty cycle for TESTING PURPOSES ONLY!!! This setting will not be saved to config.", "(0-1)")
#else
    Command("AT+DD")
#endif
{
    _queryable = true;
}

uint32_t CmdDisableDutyCycle::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getDisableDutyCycle());
    } else if (args.size() == 2) {
        CommandTerminal::Dot()->setDisableDutyCycle(args[1] == "1");
    }

    return 0;
}

bool CmdDisableDutyCycle::verify(const std::vector<std::string>& args) {

    if (args.size() > 2) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::setErrorMessage("Invalid parameter, expects (0-1)");
#endif
        return false;
    }

    if (args.size() == 2) {
        int bytes;

        sscanf(args[1].c_str(), "%d", &bytes);

        if (!(bytes == 0 || bytes == 1)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0-1)");
#endif
            return false;
        }
    }

    return true;
}
