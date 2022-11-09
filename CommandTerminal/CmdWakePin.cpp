#include "CmdWakePin.h"

CmdWakePin::CmdWakePin()
#if defined(TARGET_MTS_MDOT_F411RE)
: Command("Wake Pin", "AT+WP", "Wakeup DIO pin of sleep mode (1-8) (default: DI8, 1:DIN), deep-sleep uses DIO7", "(1-8)")
#else
: Command("Wake Pin", "AT+WP", "", "(1-6)")
#endif /* TARGET_MTS_MDOT_F411RE */
   {
    _queryable = true;
}

uint32_t CmdWakePin::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%s\r\n", mDot::pinName2Str(CommandTerminal::Dot()->getWakePin()).c_str());
    } else if (args.size() == 2) {
        int pin;
        sscanf(args[1].c_str(), "%d", &pin);

        CommandTerminal::Dot()->setWakePin(mDot::pinNum2Name(pin));
    }

    return 0;
}

bool CmdWakePin::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int pin;
        if (sscanf(args[1].c_str(), "%d", &pin) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

#if defined(TARGET_MTS_MDOT_F411RE)
        if (pin < 1 || pin > 8) {
            CommandTerminal::setErrorMessage("Invalid pin, expects (1-8)");
#else
        if (pin < 1 || pin > 6) {
            CommandTerminal::setErrorMessage("Invalid pin, expects (1-6)");
#endif /* TARGET_MTS_MDOT_F411RE */
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
