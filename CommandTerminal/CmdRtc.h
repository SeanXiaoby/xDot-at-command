#ifndef __CMDRTC_H__
#define __CMDRTC_H__

#include "Command.h"

class CmdRtc : public Command {

public:

    CmdRtc();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif // __CMDRTC_H__
