#ifndef __CMDRXFREQUENCY_H__
#define __CMDRXFREQUENCY_H__

#include "Command.h"

class CommandTerminal;

class CmdRxFrequency : public Command {

public:

    CmdRxFrequency();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDRXFREQUENCY_H__
