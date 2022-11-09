#ifndef __CMDENCRYPTION_H__
#define __CMDENCRYPTION_H__

#include "Command.h"

class CommandTerminal;

class CmdEncryption : public Command {

public:

    CmdEncryption();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDENCRYPTION_H__
