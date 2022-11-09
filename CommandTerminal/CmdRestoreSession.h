#ifndef __CMDRESTORESESSION_H__
#define __CMDRESTORESESSION_H__

#include "Command.h"

class CommandTerminal;

class CmdRestoreSession : public Command {

public:

    CmdRestoreSession();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }
    
private:   
    
};

#endif // __CMDRESTORESESSION_H__
