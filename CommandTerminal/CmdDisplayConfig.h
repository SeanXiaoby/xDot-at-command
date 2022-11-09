#ifndef __CMDDISPLAYCONFIG_H__
#define __CMDDISPLAYCONFIG_H__

#if MTS_CMD_TERM_VERBOSE
#include "Command.h"

class CmdDisplayConfig : public Command {

public:

    CmdDisplayConfig();
    virtual uint32_t action(const std::vector<std::string>& args);

private:

};

#endif // MTS_CMD_TERM_VERBOSE

#endif // __CMDDISPLAYCONFIG_H__
