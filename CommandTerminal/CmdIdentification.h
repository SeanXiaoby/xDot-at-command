#ifndef __CMDIDENTIFICATION_H__
#define __CMDIDENTIFICATION_H__

#include "Command.h"

class CmdIdentification : public Command {

public:

    CmdIdentification();   
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args) { return true; }
    
private:
    
};

#endif // __CMDIDENTIFICATION_H__
