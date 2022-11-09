
#ifndef __CMDWAKETIMEOUT_H__
#define __CMDWAKETIMEOUT_H__

#include "Command.h"

class CommandTerminal;

class CmdWakeTimeout : public Command {

public:

    CmdWakeTimeout();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDWAKETIMEOUT_H__
