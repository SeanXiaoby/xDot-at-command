#ifndef __CMDRESETSTATS_H__
#define __CMDRESETSTATS_H__

#include "Command.h"

class CmdResetStats : public Command {

public:

    CmdResetStats();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:

};

#endif // __CMDRESETSTATS_H__
