
#ifndef __CMDMACCMD_H__
#define __CMDMACCMD_H__

#include "Command.h"

class CmdMacCmd : public Command {

public:

    CmdMacCmd();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
};

#endif // __CMDMACCMD_H__
