
#ifndef __CMDERASEFLASH_H__
#define __CMDERASEFLASH_H__

#include "Command.h"

class CommandTerminal;

class CmdEraseFlash : public Command {

public:

    CmdEraseFlash();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
};

#endif // __CMDERASEFLASH_H__
