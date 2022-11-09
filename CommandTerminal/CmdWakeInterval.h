
#ifndef __CMDWAKEINTERVAL_H__
#define __CMDWAKEINTERVAL_H__

#include "Command.h"

class CommandTerminal;

class CmdWakeInterval : public Command {

public:

    CmdWakeInterval();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDWAKEINTERVAL_H__
