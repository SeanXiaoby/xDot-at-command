
#ifndef __CMDWRITEPROTECTEDCONFIG_H__
#define __CMDWRITEPROTECTEDCONFIG_H__

#include "Command.h"

class CmdWriteProtectedConfig : public Command {

public:

    CmdWriteProtectedConfig();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
};

#endif // __CMDWRITEPROTECTEDCONFIG_H__
