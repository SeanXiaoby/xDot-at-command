#ifndef __CMDJOINRETRIES_H_
#define __CMDJOINRETRIES_H_

#include "Command.h"

class CmdJoinRetries : public Command {

public:

    CmdJoinRetries();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINRETRIES_H_
