
#ifndef __CMDTXWAIT_H__
#define __CMDTXWAIT_H__

#include "Command.h"

class CommandTerminal;

class CmdTxWait : public Command {

public:

    CmdTxWait();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTXWAIT_H__
