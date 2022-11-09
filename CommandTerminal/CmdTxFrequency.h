#ifndef __CMDTXFREQUENCY_H__
#define __CMDTXFREQUENCY_H__

#include "Command.h"

class CommandTerminal;

class CmdTxFrequency : public Command {

public:

    CmdTxFrequency();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDTXFREQUENCY_H__
