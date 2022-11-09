#ifndef __CMDDEFAULTFREQUENCYBAND_H__
#define __CMDDEFAULTFREQUENCYBAND_H__

#include "Command.h"

class CommandTerminal;

class CmdDefaultFrequencyBand : public Command {

public:

    CmdDefaultFrequencyBand();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   

    
};

#endif // __CMDDEFAULTFREQUENCYBAND_H__
