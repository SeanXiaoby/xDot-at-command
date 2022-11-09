#ifndef __CMDDEVICEID_H__
#define __CMDDEVICEID_H__

#include "Command.h"

class CommandTerminal;

class CmdDeviceId : public Command {

public:

    CmdDeviceId();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
    
};

#endif // __CMDDEVICEID_H__
