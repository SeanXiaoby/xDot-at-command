#ifndef __CMDSERIALCLEARONERROR_H__
#define __CMDSERIALCLEARONERROR_H__

#include "Command.h"

class CmdSerialClearOnError : public Command {

public:

    CmdSerialClearOnError();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDSERIALCLEARONERROR_H__
