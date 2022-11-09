#include "CmdWakePin.h"

CmdWakePin::CmdWakePin() :
#if MTS_CMD_TERM_VERBOSE
    Command("Wake Pin", "AT+WP", "Wakeup DIO pin of sleep mode (1-8) (default: DI8, 1:DIN), deep-sleep uses DIO7", "(1-8),(0:NOPULL,1:PULLUP,2:PULLDOWN),(0:ANY,1:RISE,2:FALL)")
#else
    Command("AT+WP")
#endif
{
    _queryable = true;
}

uint32_t CmdWakePin::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        static char modes[][10] = {
            "NOPULL", "PULLUP", "PULLDOWN", "OTHER"
        };
        static char triggers[][5] = {
            "ANY", "RISE", "FALL", "?"
        };

        int mode;
        PinMode pm = CommandTerminal::Dot()->getWakePinMode();
        switch (pm) {
        case PullNone:
            mode = 0;
            break;
        case PullUp:
            mode = 1;
            break;
        case PullDown:
            mode = 2;
            break;
        default:
            mode = 3;
            break;
        }

        int trigger = CommandTerminal::Dot()->getWakePinTrigger();
        if (trigger > 2) {
            trigger = 3;
        }
       
        CommandTerminal::Serial()->writef("%s,%s,%s\r\n", 
            mDot::pinName2Str(CommandTerminal::Dot()->getWakePin()).c_str(), 
            modes[mode],
            triggers[trigger]);
        
    } else if (args.size() >= 2) {
        int pin;
        sscanf(args[1].c_str(), "%d", &pin);

        CommandTerminal::Dot()->setWakePin(mDot::pinNum2Name(pin));

        if (args.size() > 2) {
            int mode;
            sscanf(args[2].c_str(), "%d", &mode);
            PinMode pm;
            switch (mode) {
            case 0:
                pm = PullNone;
                break;
            case 1:
                pm = PullUp;
                break;
            case 2:
                pm = PullDown;
                break;
            default:
                pm = PullDefault;
                break;
            }
            CommandTerminal::Dot()->setWakePinMode(pm);
        }

        if (args.size() > 3) {
            int trigger;
            sscanf(args[3].c_str(), "%d", &trigger);
            CommandTerminal::Dot()->setWakePinTrigger(trigger);
        }
    }

    return 0;
}

bool CmdWakePin::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() >= 2) {
        int pin;
        if (sscanf(args[1].c_str(), "%d", &pin) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

#if defined(TARGET_MTS_MDOT_F411RE)
        if (pin < 1 || pin > 8) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid pin, expects (1-8)");
#endif
#else
        if (pin < 1 || pin > 6) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid pin, expects (1-6)");
#endif
#endif /* TARGET_MTS_MDOT_F411RE */
            return false;
        }

        if (args.size() > 2) {
            int mode;
            if (sscanf(args[2].c_str(), "%d", &mode) != 1) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid argument");
#endif
                return false;
            }

            if (mode < 0 || mode > 2) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid pin mode, expects (0-2)");
#endif
                return false;
            }
        }

        if (args.size() > 3) {
            int trigger;
            if (sscanf(args[3].c_str(), "%d", &trigger) != 1) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid argument");
#endif
                return false;
            }

            if (trigger < 0 || trigger > 2) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid trigger, expects (0-2)");
#endif
                return false;
            }
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
