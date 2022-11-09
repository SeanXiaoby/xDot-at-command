#ifndef __CMDTIMEONAIR_H__
#define __CMDTIMEONAIR_H__

#include "Command.h"



class CommandTerminal;

class CmdTimeOnAir : public Command {

public:

    CmdTimeOnAir();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDTIMEONAIR_H__
