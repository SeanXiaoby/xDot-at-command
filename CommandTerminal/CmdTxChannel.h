#ifndef __CMDTXCHANNEL_H__
#define __CMDTXCHANNEL_H__

#include "Command.h"

class CommandTerminal;

class CmdTxChannel : public Command {

public:

    CmdTxChannel();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTXCHANNEL_H__
