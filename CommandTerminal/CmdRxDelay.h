/*
 * CmdRxDelay.h
 *
 *  Created on: Nov 4, 2015
 *      Author: jreiss
 */

#ifndef CMDRXDELAY_H_
#define CMDRXDELAY_H_

#include "Command.h"

class CmdRxDelay : public Command {
public:
    CmdRxDelay();
    virtual ~CmdRxDelay();

    virtual uint32_t action(const std::vector<std::string>& args);
    virtual bool verify(const std::vector<std::string>& args);

private:
    
};

#endif /* CMDRXDELAY_H_ */
