#include "CmdDummy.h"

CmdDummy::CmdDummy(const char* name, const char* txt, const char* dsc, const char* usage) :
    Command(name, txt, dsc, usage) {

}


uint32_t CmdDummy::action(const std::vector<std::string>& args) {
    return 0;
}

bool CmdDummy::verify(const std::vector<std::string>& args) {
    if (args.size() > 1) {
        CommandTerminal::setErrorMessage("Invalid Arguments");
        return false;
    }

    return true;
}
