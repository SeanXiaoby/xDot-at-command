
#ifndef __CMDWAKEPIN_H__
#define __CMDWAKEPIN_H__

#include "Command.h"

class CommandTerminal;

class CmdWakePin : public Command {

public:

    CmdWakePin();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDWAKEPIN_H__
