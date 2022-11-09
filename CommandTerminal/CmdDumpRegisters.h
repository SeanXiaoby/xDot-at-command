#ifndef __CMDDUMPREGISTERS_H__
#define __CMDDUMPREGISTERS_H__

#include "Command.h"

class CommandTerminal;

class CmdDumpRegisters : public Command {

public:

    CmdDumpRegisters();   
    virtual uint32_t action(const std::vector<std::string>& args);
    
private:    
    
};

#endif // __CMDDUMPREGISTERS_H__
