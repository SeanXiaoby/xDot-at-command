/*
 * CmdJoinDelay.h
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#ifndef CMDJOINDELAY_H_
#define CMDJOINDELAY_H_

#include "Command.h"

class CmdJoinDelay : public Command {
public:
    CmdJoinDelay();
    virtual ~CmdJoinDelay();

    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif /* CMDJOINDELAY_H_ */
