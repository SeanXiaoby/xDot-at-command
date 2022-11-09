
#ifndef __CMDWAKEDELAY_H__
#define __CMDWAKEDELAY_H__

#include "Command.h"

class CommandTerminal;

class CmdWakeDelay : public Command {

public:

    CmdWakeDelay();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDWAKEDELAY_H__
