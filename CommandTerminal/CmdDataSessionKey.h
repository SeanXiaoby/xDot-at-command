#ifndef __CMDDATASESSIONKEY_H__
#define __CMDDATASESSIONKEY_H__

#include "Command.h"

class CommandTerminal;

class CmdDataSessionKey : public Command {

public:

    CmdDataSessionKey();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDDATASESSIONKEY_H__
