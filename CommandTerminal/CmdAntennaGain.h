
#ifndef __CMDANTENNAGAIN_H__
#define __CMDANTENNAGAIN_H__

#include "Command.h"

class CommandTerminal;

class CmdAntennaGain : public Command {

public:

    CmdAntennaGain();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDANTENNAGAIN_H__
