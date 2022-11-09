#ifndef __CMDDEBUGBAUDRATE_H__
#define __CMDDEBUGBAUDRATE_H__

#include "Command.h"

class CmdDebugBaudRate : public Command {

public:

    CmdDebugBaudRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDDEBUGBAUDRATE_H__
