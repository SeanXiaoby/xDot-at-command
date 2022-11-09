#ifndef __CMDPINGPERIOD_H__
#define __CMDPINGPERIOD_H__

#include "Command.h"

class CommandTerminal;

class CmdPingPeriod : public Command {

public:

    CmdPingPeriod();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDPINGPERIOD_H__
