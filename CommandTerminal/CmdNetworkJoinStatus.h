#ifndef __CMDNETWORKJOINSTATUS_H__
#define __CMDNETWORKJOINSTATUS_H__

#include "Command.h"

class CommandTerminal;

class CmdNetworkJoinStatus : public Command {

public:

    CmdNetworkJoinStatus();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:   
    
};

#endif // __CMDNETWORKJOINSTATUS_H__
