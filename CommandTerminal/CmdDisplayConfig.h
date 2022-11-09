#ifndef __CMDDISPLAYCONFIG_H__
#define __CMDDISPLAYCONFIG_H__

#include "Command.h"

class CmdDisplayConfig : public Command {

public:

    CmdDisplayConfig();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:

};

#endif // __CMDDISPLAYCONFIG_H__
