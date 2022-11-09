#ifndef __CMDNETWORKLINKCHECK_H__
#define __CMDNETWORKLINKCHECK_H__

#include "Command.h"

class CommandTerminal;

class CmdNetworkLinkCheck : public Command {

public:

    CmdNetworkLinkCheck();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }
    
private:   
    
};

#endif // __CMDNETWORKLINKCHECK_H__
