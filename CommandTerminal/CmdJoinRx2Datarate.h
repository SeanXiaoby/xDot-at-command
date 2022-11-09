#ifndef __CMDJOINRX2DATARATE_H__
#define __CMDJOINRX2DATARATE_H__

#include "Command.h"

class CmdJoinRx2Datarate : public Command {

public:

    CmdJoinRx2Datarate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINRX2DATARATE_H__
