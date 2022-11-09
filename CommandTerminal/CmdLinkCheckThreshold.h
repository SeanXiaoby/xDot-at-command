#ifndef __CMDLINKCHECKHTHRESHOLD_H__
#define __CMDLINKCHECKHTHRESHOLD_H__

#include "Command.h"

class CommandTerminal;

class CmdLinkCheckThreshold : public Command {

public:

    CmdLinkCheckThreshold();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDLINKCHECKHTHRESHOLD_H__
