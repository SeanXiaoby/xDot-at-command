
#ifndef __CMDSENDSTRINGONINTERVAL_H__
#define __CMDSENDSTRINGONINTERVAL_H__

#include "Command.h"

class CmdSendStringOnInterval : public Command {

public:

    CmdSendStringOnInterval();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    

};


#endif // __CMDSENDSTRINGONINTERVAL_H__
