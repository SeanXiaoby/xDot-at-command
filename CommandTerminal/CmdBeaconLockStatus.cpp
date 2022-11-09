#include "CmdBeaconLockStatus.h"

CmdBeaconLockStatus::CmdBeaconLockStatus()
    : Command("Beacon Lock Status", "AT+BLS",
#if defined(TARGET_MTS_MDOT_F411RE)
        "Get the lock status of the beacon (0: not locked, 1: locked)",
#else
        "",
#endif
        "NONE")
{
    _queryable = true;
}

uint32_t CmdBeaconLockStatus::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n",
                                          CommandTerminal::Events()->BeaconLocked ? 1 : 0);
    }

    return 0;
}

bool CmdBeaconLockStatus::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
