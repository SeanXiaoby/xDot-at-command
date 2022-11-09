#ifndef __CMDDISPLAYSTATS_H__
#define __CMDDISPLAYSTATS_H__

#if MTS_CMD_TERM_VERBOSE
#include "Command.h"

class CmdDisplayStats : public Command {

public:

    CmdDisplayStats();
    virtual uint32_t action(const std::vector<std::string>& args);

private:

};

#endif // MTS_CMD_TERM_VERBOSE

#endif // __CMDDISPLAYSTATS_H__
