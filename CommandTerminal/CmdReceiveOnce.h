
#ifndef __CMDRECEIVEONCE_H__
#define __CMDRECEIVEONCE_H__

#include "Command.h"

class CommandTerminal;

class CmdReceiveOnce : public Command {

public:

    CmdReceiveOnce();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:    
    
};

#endif // __CMDRECEIVEONCE_H__
