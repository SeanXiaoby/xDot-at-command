#ifndef __CMDLBT_H_
#define __CMDLBT_H_

#include "Command.h"

class CommandTerminal;

class CmdLbt : public Command {

public:

    CmdLbt();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDLBT_H_
