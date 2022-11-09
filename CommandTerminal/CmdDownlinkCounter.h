#ifndef __CMDDOWNLINKCOUNTER_H_
#define __CMDDOWNLINKCOUNTER_H_

#include "Command.h"

class CmdDownlinkCounter : public Command {

public:

    CmdDownlinkCounter();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDDOWNLINKCOUNTER_H_
