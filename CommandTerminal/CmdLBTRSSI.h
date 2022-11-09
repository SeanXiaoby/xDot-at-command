#ifndef __CmdLBTRSSI_H__
#define __CmdLBTRSSI_H__

#include "Command.h"

class CommandTerminal;

class CmdLBTRSSI : public Command {

public:

    CmdLBTRSSI();
    virtual uint32_t action(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CmdLBTRSSI_H__
