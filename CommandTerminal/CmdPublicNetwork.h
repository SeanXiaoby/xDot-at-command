#ifndef __CMDPUBLICNETWORK_H__
#define __CMDPUBLICNETWORK_H__

#include "Command.h"

class CommandTerminal;

class CmdPublicNetwork : public Command {

public:

    CmdPublicNetwork();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:
    
};

#endif // __CMDPUBLICNETWORK_H__
