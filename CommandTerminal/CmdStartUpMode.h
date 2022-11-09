#ifndef __CMDSTARTUPMODE_H__
#define __CMDSTARTUPMODE_H__

#include "Command.h"

class CommandTerminal;

class CmdStartUpMode : public Command {

public:

    CmdStartUpMode();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDSTARTUPMODE_H__
