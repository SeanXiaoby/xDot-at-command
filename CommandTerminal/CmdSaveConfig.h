
#ifndef __CmdSaveConfig_H__
#define __CmdSaveConfig_H__

#include "Command.h"

class CmdSaveConfig : public Command {

public:

    CmdSaveConfig();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
};

#endif // __CmdSaveConfig_H__
