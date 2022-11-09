
#ifndef __CMDREPAIRFLASH_H__
#define __CMDREPAIRFLASH_H__

#include "Command.h"

class CommandTerminal;

class CmdRepairFlash : public Command {

public:

    CmdRepairFlash();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
};

#endif // __CMDREPAIRFLASH_H__
