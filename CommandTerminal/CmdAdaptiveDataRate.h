
#ifndef __CMDCMDADAPTIVEDATARATE_H__
#define __CMDCMDADAPTIVEDATARATE_H__

#include "Command.h"

class CommandTerminal;

class CmdAdaptiveDataRate : public Command {

public:

    CmdAdaptiveDataRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDCMDADAPTIVEDATARATE_H__
