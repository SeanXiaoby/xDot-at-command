#ifndef __CMDSERIALBAUDRATE_H__
#define __CMDSERIALBAUDRATE_H__

#include "Command.h"

class CmdSerialBaudRate : public Command {

public:

    CmdSerialBaudRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDSERIALBAUDRATE_H__
