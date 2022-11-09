
#ifndef __CMDSENDBINARY_H__
#define __CMDSENDBINARY_H__

#include "Command.h"

class CmdSendBinary : public Command {

public:

    CmdSendBinary();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
    
};

#endif // __CMDSENDBINARY_H__
