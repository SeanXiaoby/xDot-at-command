
#ifndef __CMDRECEIVECONTINUOUS_H__
#define __CMDRECEIVECONTINUOUS_H__

#include "Command.h"

class CommandTerminal;

class CmdReceiveContinuous : public Command {

public:

    CmdReceiveContinuous();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:    
    
};

#endif // __CMDRECEIVECONTINUOUS_H__
