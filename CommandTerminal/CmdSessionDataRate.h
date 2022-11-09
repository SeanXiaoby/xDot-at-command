#ifndef __CMDSESSIONDATARATE_H__
#define __CMDSESSIONDATARATE_H__

#include "Command.h"

class CommandTerminal;

class CmdSessionDataRate : public Command {

public:

    CmdSessionDataRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __CMDSESSIONDATARATE_H__
