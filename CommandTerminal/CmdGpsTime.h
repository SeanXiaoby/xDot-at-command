#ifndef __CMDGPSTIME_H__
#define __CMDGPSTIME_H__

#include "Command.h"

class CommandTerminal;

class CmdGpsTime : public Command {

public:

    CmdGpsTime();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDGPSTIME_H__
