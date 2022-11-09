/*
 * CmdDeviceClass.h
 *
 *  Created on: Nov 9, 2015
 *      Author: jreiss
 */

#ifndef CMDDEVICECLASS_H_
#define CMDDEVICECLASS_H_

#include "Command.h"

class CmdDeviceClass: public Command {
    public:
        CmdDeviceClass();
        virtual ~CmdDeviceClass();

        virtual uint32_t action(const std::vector<std::string>& args);
        virtual bool verify(const std::vector<std::string>& args);

    private:

};

#endif /* CMDDEVICECLASS_H_ */
