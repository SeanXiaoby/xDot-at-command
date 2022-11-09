
#ifndef __CMDSENDSTRING_H__
#define __CMDSENDSTRING_H__

#include "Command.h"

class CmdSendString : public Command {

public:

    CmdSendString();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
    
};

#endif // __CMDSENDSTRING_H__
