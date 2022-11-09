#ifndef __CMDCHANNELMASK_H__
#define __CMDCHANNELMASK_H__

#include "Command.h"

class CommandTerminal;

class CmdChannelMask : public Command {

public:

    CmdChannelMask();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
};

#endif // __CMDCHANNELMASK_H__
