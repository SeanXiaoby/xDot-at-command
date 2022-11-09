#ifndef CMDBATTERYLEVEL_H_
#define CMDBATTERYLEVEL_H_

#include "Command.h"

class CmdBatteryLevel : public Command {
    public:
        CmdBatteryLevel();

        virtual uint32_t action(const std::vector<std::string>& args);
        virtual bool verify(const std::vector<std::string>& args);

    private:

};

#endif
