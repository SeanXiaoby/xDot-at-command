#include "CmdBatteryLevel.h"

CmdBatteryLevel::CmdBatteryLevel() :
#if MTS_CMD_TERM_VERBOSE
    Command("Battery Level", "AT+BAT", "Battery Level of dot (0 - 255)", "(0-255)")
#else
    Command("AT+BAT")
#endif
{
    _queryable = true;
}

uint32_t CmdBatteryLevel::action(const std::vector<std::string>& args) {
    int battery_level;

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::getBatteryLevel());
    } else if (args.size() == 2) {
        sscanf(args[1].c_str(), "%d", &battery_level);
        CommandTerminal::setBatteryLevel(battery_level);
    }
    return 0;
}

bool CmdBatteryLevel::verify(const std::vector<std::string>& args) {
    int battery_level;

    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        if (sscanf(args[1].c_str(), "%d", &battery_level) == 1) {
            if (battery_level > 255 || battery_level < 0) {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid battery level, expects (0-255)");
#endif
                return false;
            }
            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

