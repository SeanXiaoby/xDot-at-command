
#ifndef __CMDRESETCPU_H__
#define __CMDRESETCPU_H__

#include "Command.h"

class CmdResetCpu : public Command {

public:

    CmdResetCpu();   
    virtual uint32_t action(const std::vector<std::string>& args);
    
private:

};

#endif // __CMDRESETCPU_H__
