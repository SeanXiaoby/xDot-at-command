#ifndef __CMDFOTA_H__
#define __CMDFOTA_H__

#include "Command.h"

class CommandTerminal;

class CmdFota : public Command {

    public:

        CmdFota();
        virtual uint32_t action(const std::vector<std::string>& args);
        virtual bool verify(const std::vector<std::string>& args);
};

#endif // __CMDFOTA_H__

