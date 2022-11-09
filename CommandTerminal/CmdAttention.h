#ifndef __CMDATTENTION_H__
#define __CMDATTENTION_H__

#include "Command.h"

class CmdAttention : public Command {

public:

    CmdAttention();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
};

#endif // __CMDATTENTION_H__
