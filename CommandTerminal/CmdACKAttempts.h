
#ifndef __CMDACKATTEMPTS_H__
#define __CMDACKATTEMPTS_H__

#include "Command.h"

class CommandTerminal;

class CmdACKAttempts : public Command {

public:

    CmdACKAttempts();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDACKATTEMPTS_H__
