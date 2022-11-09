#include "CmdBatteryLevel.h"

CmdBatteryLevel::CmdBatteryLevel()
    :
        Command("Battery Level", "AT+BAT",
#if defined(TARGET_MTS_MDOT_F411RE)
        "Battery Level of dot (0 - 255)",
#else
        "",
#endif
         "(0-255)")
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
                CommandTerminal::setErrorMessage("Invalid battery level, expects (0-255)");
                return false;
            }
            return true;
        }
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

