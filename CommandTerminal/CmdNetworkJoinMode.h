#ifndef __CMDNETWORKJOINMODE_H__
#define __CMDNETWORKJOINMODE_H__

#include "Command.h"

class CommandTerminal;

class CmdNetworkJoinMode : public Command {

public:

    CmdNetworkJoinMode();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDNETWORKJOINMODE_H__
