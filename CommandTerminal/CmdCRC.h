#ifndef __CMDCRC_H__
#define __CMDCRC_H__

#include "Command.h"

class CommandTerminal;

class CmdCRC : public Command {

public:

    CmdCRC();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDCRC_H__
