#include "CmdDummy.h"

#if MTS_CMD_TERM_VERBOSE
CmdDummy::CmdDummy(const char* name, const char* txt, const char* dsc, const char* usage) :
   Command(name, txt, dsc, usage) 
#else
CmdDummy::CmdDummy(const char* txt) :
   Command(txt) 
#endif
{

}


uint32_t CmdDummy::action(const std::vector<std::string>& args) {
    return 0;
}
