#ifndef __CMDFREEMEMORY_H__
#define __CMDFREEMEMORY_H__

#include "Command.h"

class CommandTerminal;

class CmdFreeMemory : public Command {

public:

    CmdFreeMemory();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDFREEMEMORY_H__
