
#ifndef __CMDREPEAT_H__
#define __CMDREPEAT_H__

#include "Command.h"

class CommandTerminal;

class CmdRepeat : public Command {

public:

    CmdRepeat();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDREPEAT_H__
