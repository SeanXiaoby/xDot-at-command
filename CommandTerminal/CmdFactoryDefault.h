#ifndef __CmdFactoryDefault_H__
#define __CmdFactoryDefault_H__

#include "Command.h"

class CmdFactoryDefault : public Command {

public:

    CmdFactoryDefault();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }

private:
};

#endif // __CmdFactoryDefault_H__
