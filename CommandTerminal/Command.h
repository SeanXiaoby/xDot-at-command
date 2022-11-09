/**
 ******************************************************************************
 * File Name          : Command.h
 * Date               : 18/04/2014 10:57:12
 * Description        : This file provides code for command line prompt
 ******************************************************************************
 *
 * COPYRIGHT(c) 2014 MultiTech Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "mbed.h"
#include "mDot.h"
#include "MTSSerial.h"
#include "MTSText.h"
#include <cstdlib>
#include <string>
#include <vector>
#include "limits.h"
#include "mts_at_debug.h"
#include "CommandTerminal.h"


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __command_H
#define __command_H

#define KEY_LENGTH 16
#define EUI_LENGTH 8
#define PASSPHRASE_LENGTH 128

class Command
{
public:
    Command();
    Command(const char* name, const char* text, const char* desc, const char* usage);
    virtual ~Command() {};

    const char* name() const { return _name; };
    const char* text() const { return _text; };
    const char* desc() const { return _desc; };
    const std::string help() const {
        return std::string(text()) + ": " + std::string(desc());
    };

    virtual uint32_t action(const std::vector<std::string>& args) = 0;
    virtual bool verify(const std::vector<std::string>& args) = 0;

    std::string usage() const;
    const std::string& errorMessage() const;
    bool queryable() const;

    static const char newline[];
    static void readByteArray(const std::string& input, std::vector<uint8_t>& out, size_t len);

    static bool isHexString(const std::string& str, size_t bytes);
    static bool isBaudRate(uint32_t baud);

protected:
    bool _queryable;

private:
    const char* _name;
    const char* _text;
    const char* _desc;
    const char* _usage;
};

#endif /*__ command_H */

/************************ (C) COPYRIGHT MultiTech Systems, Inc *****END OF FILE****/
