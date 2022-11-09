#ifndef __CMDDUTYCYCLE_H__
#define __CMDDUTYCYCLE_H__

#include "Command.h"

class CommandTerminal;

class CmdDutyCycle : public Command {

public:

    CmdDutyCycle();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDDUTYCYCLE_H__
