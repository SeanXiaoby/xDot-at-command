#ifndef __CMDJOINREQUEST_H__
#define __CMDJOINREQUEST_H__

#include "Command.h"

class CmdJoinRequest : public Command {

public:

    CmdJoinRequest();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINREQUEST_H__
