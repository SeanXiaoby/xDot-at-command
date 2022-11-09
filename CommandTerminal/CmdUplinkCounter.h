#ifndef __CMDUPLINKCOUNTER_H_
#define __CMDUPLINKCOUNTER_H_

#include "Command.h"

class CmdUplinkCounter : public Command {

public:

    CmdUplinkCounter();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDUPLINKCOUNTER_H_
