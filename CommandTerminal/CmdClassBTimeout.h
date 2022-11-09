#ifndef __CMDCLASSBTIMEOUT_H__
#define __CMDCLASSBTIMEOUT_H__

#include "Command.h"

class CommandTerminal;

class CmdClassBTimeout : public Command {

public:

    CmdClassBTimeout();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDCLASSBTIMEOUT_H__
