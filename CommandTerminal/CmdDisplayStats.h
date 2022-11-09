#ifndef __CMDDISPLAYSTATS_H__
#define __CMDDISPLAYSTATS_H__

#include "Command.h"

class CmdDisplayStats : public Command {

public:

    CmdDisplayStats();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:

};

#endif // __CMDDISPLAYSTATS_H__
