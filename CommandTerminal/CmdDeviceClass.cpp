#include "CmdDeviceClass.h"
#include "CommandTerminal.h"

CmdDeviceClass::CmdDeviceClass() :
#if MTS_CMD_TERM_VERBOSE
    Command("Device Class", "AT+DC", "Device class (A,B,C)", "(A,B,C)")
#else
    Command("AT+DC")
#endif
{

    _queryable = true;
}

CmdDeviceClass::~CmdDeviceClass() {

}

uint32_t CmdDeviceClass::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->getClass().c_str());
    } else {
        if (CommandTerminal::Dot()->setClass(args[1]) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdDeviceClass::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if ((args.size() == 2) && (args[1].size() == 1)) {
        char dev_class = args[1][0];
        if (dev_class == 'A' || dev_class == 'B' || dev_class == 'C' ||
            dev_class == 'a' || dev_class == 'b' || dev_class == 'c') {
            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
