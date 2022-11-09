#ifndef __CMDLOGLEVEL_H__
#define __CMDLOGLEVEL_H__

#include "Command.h"

class CommandTerminal;

class CmdLogLevel : public Command {

public:

    CmdLogLevel();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDLOGLEVEL_H__
