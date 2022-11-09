
#ifndef __CMDSENDCONTINUOUS_H__
#define __CMDSENDCONTINUOUS_H__

#include "Command.h"

class CmdSendContinuous : public Command {

public:

    CmdSendContinuous();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
    
};

#endif // __CMDSENDCONTINUOUS_H__
