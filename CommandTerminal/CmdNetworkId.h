#ifndef __CMDNETWORKID_H__
#define __CMDNETWORKID_H__

#include "Command.h"

class CommandTerminal;

class CmdNetworkId : public Command {

public:

    CmdNetworkId();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDNETWORKID_H__
