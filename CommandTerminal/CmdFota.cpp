#include "CmdFota.h"
CmdFota::CmdFota() : Command("Fota and Multicast", "AT+FOTA",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Enable/disable Fota (0: off, 1: on, 2: reset, 3: time",
#else
    "",
#endif
    "(0,1,2,3)") {
    _queryable = true;
}

uint32_t CmdFota::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%d\r\n",Fota::getInstance()->enable());
    }
    else if (args.size() == 2) {
        if (args[1] == "0") {
            Fota::getInstance()->enable(false);
        } else if(args[1] == "1") {
            Fota::getInstance()->enable(true);
        } else if(args[1] == "2") {
            Fota::getInstance()->reset();
        } else if(args[1] == "3") {
           CommandTerminal::Serial()->writef("%d\r\n",Fota::getInstance()->timeToStart());
        }
    }
    return 0;
}

bool CmdFota::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        if (args[1] != "3" && args[1] != "2" && args[1] != "1" && args[1] != "0") {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on, 2: reset, 3: time)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

