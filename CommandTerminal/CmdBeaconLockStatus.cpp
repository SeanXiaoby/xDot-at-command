#include "CmdBeaconLockStatus.h"

CmdBeaconLockStatus::CmdBeaconLockStatus() :
#if MTS_CMD_TERM_VERBOSE
    Command("Beacon Lock Status", "AT+BLS", "Get the lock status of the beacon (0: not locked, 1: locked)", "NONE")
#else
    Command("AT+BLS")
#endif
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
