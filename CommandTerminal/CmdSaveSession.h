#ifndef __CMDSAVESESSION_H__
#define __CMDSAVESESSION_H__

#include "Command.h"

class CommandTerminal;

class CmdSaveSession : public Command {

public:

    CmdSaveSession();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }
    
private:   
    
};

#endif // __CMDSAVESESSION_H__
