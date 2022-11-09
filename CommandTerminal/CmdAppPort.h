/*
 * CmdAppPort.h
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#ifndef CMDAPPPORT_H_
#define CMDAPPPORT_H_

#include "Command.h"

class CmdAppPort : public Command {
public:
    CmdAppPort();
    virtual ~CmdAppPort();

    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif /* CMDAPPPORT_H_ */
