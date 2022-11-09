#ifndef __CMDMULTICASTRX_H__
#define __CMDMULTICASTRX_H__

#include "Command.h"

class CommandTerminal;

class CmdMulticastRx : public Command {

public:

    CmdMulticastRx();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDMULTICASTRX_H__
