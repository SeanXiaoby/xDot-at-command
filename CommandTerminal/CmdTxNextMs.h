#ifndef __CMDTXNEXTMS_H__
#define __CMDTXNEXTMS_H__

#include "Command.h"



class CommandTerminal;

class CmdTxNextMs : public Command {

public:

    CmdTxNextMs();
    virtual uint32_t action(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTXNEXTMS_H__
