#ifndef __CMDSNR_H__
#define __CMDSNR_H__

#include "Command.h"

class CmdSnr : public Command {

public:

    CmdSnr();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
    
};

#endif // __CMDSNR_H__
