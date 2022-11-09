#include "CmdSendContinuous.h"
#include "CommandTerminal.h"

CmdSendContinuous::CmdSendContinuous()
: Command("Send Continuous", "AT+SENDC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Send un-modulated data continuously",
#else
    "",
#endif
    "[TIMEOUT],[FREQUENCY],[POWER]") {
}

uint32_t CmdSendContinuous::action(const std::vector<std::string>& args) {

    uint32_t timeout = 0;
    uint32_t frequency = 0;
    int8_t power = -1;

    if (args.size() > 1) {
        timeout = atoi(args[1].c_str());
    }
    if (args.size() > 2) {
        frequency = atoi(args[2].c_str());
    }
    if (args.size() > 3) {
        power = atoi(args[3].c_str());
    }

    CommandTerminal::Dot()->sendContinuous(true, timeout, frequency, power);
    CommandTerminal::Serial()->clearEscaped();
    return 0;
}

bool CmdSendContinuous::verify(const std::vector<std::string>& args) {

    return true;
}
