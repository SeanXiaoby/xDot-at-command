#ifndef __CMDRXDATARATE_H__
#define __CMDRXDATARATE_H__

#include "Command.h"

class CommandTerminal;

class CmdRxDataRate : public Command {

public:

    CmdRxDataRate();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);
    
private:   
    
};

#endif // __CMDRXDATARATE_H__
