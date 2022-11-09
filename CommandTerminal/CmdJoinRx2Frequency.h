#ifndef __CMDJOINRX2FREQUENCY_H__
#define __CMDJOINRX2FREQUENCY_H__

#include "Command.h"

class CmdJoinRx2Frequency : public Command {

public:

    CmdJoinRx2Frequency();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDJOINRX2FREQUENCY_H__
