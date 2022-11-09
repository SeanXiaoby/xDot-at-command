
#ifndef __CMDTXFREQUENCYOFFSET_H__
#define __CMDTXFREQUENCYOFFSET_H__

#include "Command.h"

class CommandTerminal;

class CmdTxFrequencyOffset : public Command {

public:
    CmdTxFrequencyOffset();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   

};

#endif // __CMDTXFREQUENCYOFFSET_H__
