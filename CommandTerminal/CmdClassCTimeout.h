#ifndef __CMDCLASSCTIMEOUT_H__
#define __CMDCLASSCTIMEOUT_H__

#include "Command.h"

class CommandTerminal;

class CmdClassCTimeout : public Command {

public:

    CmdClassCTimeout();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDCLASSCTIMEOUT_H__
