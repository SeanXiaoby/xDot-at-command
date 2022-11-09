
#ifndef __CMDRXOUTPUT_H__
#define __CMDRXOUTPUT_H__

#include "Command.h"

class CommandTerminal;

class CmdRxOutput : public Command {

public:

    CmdRxOutput();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDRXOUTPUT_H__
