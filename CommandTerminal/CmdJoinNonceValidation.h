#ifndef __CMDJOINNONCEVALIDATION_H_
#define __CMDJOINNONCEVALIDATION_H_

#include "Command.h"

class CmdJoinNonceValidation : public Command {

public:

    CmdJoinNonceValidation();
    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:

};

#endif // __CMDJOINNONCEVALIDATION_H_
