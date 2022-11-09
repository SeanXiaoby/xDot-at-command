#ifndef __CMDBEACONLOCKSTATUS_H__
#define __CMDBEACONLOCKSTATUS_H__

#include "Command.h"

class CommandTerminal;

class CmdBeaconLockStatus : public Command {

public:

    CmdBeaconLockStatus();
    virtual uint32_t action(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDBEACONLOCKSTATUS_H__
