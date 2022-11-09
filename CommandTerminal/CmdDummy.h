#ifndef __CMDDUMMY_H__
#define __CMDDUMMY_H__

#include "Command.h"

class CmdDummy : public Command {

public:

#if MTS_CMD_TERM_VERBOSE
    CmdDummy(const char* name, const char* text, const char* desc, const char* usage);
#else
    CmdDummy(const char* text);
#endif
    virtual uint32_t action(const std::vector<std::string>& args);
private:
};

#endif // __CMDDUMMY_H__
