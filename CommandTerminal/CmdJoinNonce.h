#ifndef __CMDJOINNONCE_H_
#define __CMDJOINNONCE_H_

#include "Command.h"

class CmdJoinNonce : public Command {

public:

    CmdJoinNonce();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINNONCE_H_
