#ifndef __CMDTXNEXTSIZE_H__
#define __CMDTXNEXTSIZE_H__

#include "Command.h"



class CommandTerminal;

class CmdTxNextSize : public Command {

public:

    CmdTxNextSize();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTXNEXTSIZE_H__
