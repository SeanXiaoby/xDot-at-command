#ifndef __CMDLINKCHECKCOUNT_H__
#define __CMDLINKCHECKCOUNT_H__

#include "Command.h"

class CommandTerminal;

class CmdLinkCheckCount : public Command {

public:

    CmdLinkCheckCount();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDLINKCHECKCOUNT_H__
