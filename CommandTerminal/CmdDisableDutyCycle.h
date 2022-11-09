#ifndef __CMDDISABLEDUTYCYCLE_H__
#define __CMDDISABLEDUTYCYCLE_H__

#include "Command.h"

class CommandTerminal;

class CmdDisableDutyCycle : public Command {

public:

    CmdDisableDutyCycle();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTIMEONAIR_H__
