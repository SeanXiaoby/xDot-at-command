#ifndef __CMDJOINRX1OFFSET_H__
#define __CMDJOINRX1OFFSET_H__

#include "Command.h"

class CmdJoinRx1Offset : public Command {

public:

    CmdJoinRx1Offset();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINRX1OFFSET_H__
