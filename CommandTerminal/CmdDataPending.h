
#ifndef __CMDDATAPENDING_H__
#define __CMDDATAPENDING_H__

#include "Command.h"

class CmdDataPending : public Command {

public:

    CmdDataPending();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
    
};

#endif // __CMDDATAPENDING_H__
