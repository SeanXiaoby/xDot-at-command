
#ifndef __WRITE_OTP_H__
#define __WRITE_OTP_H__

#include "Command.h"

class CommandTerminal;

class CmdWriteOtp : public Command {

public:

    CmdWriteOtp();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:   
    
};

#endif // __WRITE_OTP_H__
