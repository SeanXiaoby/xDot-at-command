#ifndef __CMDNETWORKSESSIONKEY_H__
#define __CMDNETWORKSESSIONKEY_H__

#include "Command.h"

class CommandTerminal;

class CmdNetworkSessionKey : public Command {

public:

    CmdNetworkSessionKey();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDNETWORKSESSIONKEY_H__
