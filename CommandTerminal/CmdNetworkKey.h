
#ifndef __CMDNETWORKKEY_H__
#define __CMDNETWORKKEY_H__


#include "Command.h"

class CommandTerminal;

class CmdNetworkKey : public Command {

public:

    CmdNetworkKey();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDNETWORKKEY_H__
