
#ifndef __CMDPING_H__
#define __CMDPING_H__

#include "Command.h"

class CmdPing : public Command {

public:

    CmdPing();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
    
};

#endif // __CMDPING_H__
