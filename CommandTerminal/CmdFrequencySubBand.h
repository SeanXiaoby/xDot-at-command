#ifndef __CMDFREQUENCYSUBBAND_H__
#define __CMDFREQUENCYSUBBAND_H__

#include "Command.h"

class CommandTerminal;

class CmdFrequencySubBand : public Command {

public:

    CmdFrequencySubBand();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDFREQUENCYSUBBAND_H__
