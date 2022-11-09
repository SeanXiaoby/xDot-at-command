
#ifndef __CMDGENAPPKEY_H__
#define __CMDGENAPPKEY_H__


#include "Command.h"

class CommandTerminal;

class CmdGenAppKey : public Command {

public:

    CmdGenAppKey();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDGENAPPKEY_H__
