#include "CmdDeviceClass.h"
#include "CommandTerminal.h"

CmdDeviceClass::CmdDeviceClass()
:
  Command("Device Class", "AT+DC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Device class (A,B,C)",
#else
    "",
#endif
    "(A,B,C)") {

    _queryable = true;
}

CmdDeviceClass::~CmdDeviceClass() {

}

uint32_t CmdDeviceClass::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::Dot()->getClass().c_str());
    } else {
        if (CommandTerminal::Dot()->setClass(args[1]) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdDeviceClass::verify(const std::vector<std::string>& args)
{
    std::vector<std::string> local_args(args);
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        local_args[1] = mts::Text::toUpper(args[1]);
        if (local_args[1] == "A" || local_args[1] == "B" || local_args[1] == "C") {
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
