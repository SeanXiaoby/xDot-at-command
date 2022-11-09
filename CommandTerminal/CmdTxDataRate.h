#ifndef __CMDTXDATARATE_H__
#define __CMDTXDATARATE_H__

#include "Command.h"

class CommandTerminal;

class CmdTxDataRate : public Command {

public:

    CmdTxDataRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDTXDATARATE_H__
