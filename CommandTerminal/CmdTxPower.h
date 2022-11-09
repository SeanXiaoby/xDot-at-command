#ifndef __CMDTXPOWER_H__
#define __CMDTXPOWER_H__

#include "Command.h"

class CommandTerminal;

class CmdTxPower: public Command
{

public:

    CmdTxPower();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDTXPOWER_H__
