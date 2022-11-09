#ifndef __CMDRSSI_H__
#define __CMDRSSI_H__

#include "Command.h"

class CmdRssi : public Command {

public:

    CmdRssi();
    virtual uint32_t action(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDRSSI_H__
