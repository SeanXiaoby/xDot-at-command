
#ifndef __CMDWAKEMODE_H__
#define __CMDWAKEMODE_H__

#include "Command.h"

class CommandTerminal;

class CmdWakeMode : public Command {

public:

    CmdWakeMode();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDWAKEMODE_H__
