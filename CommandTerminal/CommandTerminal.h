/**
 ******************************************************************************
 * File Name          : command.h
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
#include "ATSerial.h"
#include "MTSSerial.h"
#include "MTSSerialFlowControl.h"
#include "Commands.h"
#include "mDot.h"
#include "mDotEvent.h"
#include "Fota.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __command_terminal_H__
#define __command_terminal_H__

typedef uint32_t (*action_ptr_t)(std::vector<std::string> args);
typedef bool (*verify_ptr_t)(std::vector<std::string> args);

class Command;

class CommandTerminal {

    class RadioEvent : public mDotEvent {

    public:
        RadioEvent() : _sendAck(false) {}

        virtual ~RadioEvent() {}

        virtual void TxDone(uint8_t dr) {
            mDotEvent::TxDone(dr);

            logDebug("RadioEvent - TxDone");
        }

        virtual void TxTimeout(void) {
            mDotEvent::TxTimeout();

            logDebug("RadioEvent - TxTimeout");
        }

        virtual void JoinAccept(uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr) {
            mDotEvent::JoinAccept(payload, size, rssi, snr);

            logDebug("RadioEvent - JoinAccept");
        }

        virtual void JoinFailed(uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr) {
            mDotEvent::JoinFailed(payload, size, rssi, snr);

            logDebug("RadioEvent - JoinFailed");
        }

        virtual void PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries, uint32_t address, bool dupRx);

        virtual void RxDone(uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot);

        virtual void RxTimeout(uint8_t slot);

        virtual void Pong(int16_t m_rssi, int16_t m_snr, int16_t s_rssi, int16_t s_snr) {
            mDotEvent::Pong(m_rssi, m_snr, s_rssi, s_snr);

            logDebug("RadioEvent - Pong");
        }

        virtual void NetworkLinkCheck(int16_t m_rssi, int16_t m_snr, int16_t s_snr, uint8_t s_gateways) {
            mDotEvent::NetworkLinkCheck(m_rssi, m_snr, s_snr, s_gateways);

            logDebug("RadioEvent - NetworkLinkCheck");
        }

        virtual void ServerTime(uint32_t seconds, uint8_t sub_seconds) {
            mDotEvent::ServerTime(seconds, sub_seconds);

            Fota::getInstance()->setClockOffset(seconds);
        }

        virtual void RxError(uint8_t slot) {
            mDotEvent::RxError(slot);

            logDebug("RadioEvent - RxError");
        }

        virtual uint8_t MeasureBattery(void) {
            return CommandTerminal::getBatteryLevel();
        }

        virtual void MissedAck(uint8_t retries) {
            mDotEvent::MissedAck(retries);
        }

        bool SendAck() {
            bool val = _sendAck;
            _sendAck = false;
            return val;
        }

        bool _sendAck;
    };

public:

    enum WaitType {
        WAIT_JOIN,
        WAIT_RECV,
        WAIT_LINK,
        WAIT_SEND,
        WAIT_NA
    };

    CommandTerminal(mts::ATSerial& serial);
    virtual ~CommandTerminal();

    void init();

    // Command prompt text...
    static const char banner[];
    static const char helpline[];
    static const char prompt[];

    // Command error text...
    static const char command_error[];

    // Response texts...
    static const char help[];
    static const char cmd_error[];
    static const char newline[];
    static const char connect[];
    static const char no_carrier[];
    static const char done[];
    static const char error[];

    // Escape sequence
    static const char escape_sequence[];

    static std::string formatPacketData(const std::vector<uint8_t>& data, const uint8_t& format);
    static bool waitForEscape(int timeout, mDot* dot=NULL, WaitType wait=WAIT_NA);

    void start();

    static mts::ATSerial* Serial() {return _serialp;}

    static mDot* Dot() {return _dot;}
    static mDot* _dot;

    static const RadioEvent* Events() { return _events; }

    static void setErrorMessage(const char* message);
    static void setErrorMessage(const std::string& message);

    static uint8_t getBatteryLevel();
    static void setBatteryLevel(const uint8_t battery_level);

    static void formatPacket(uint8_t* payload, uint16_t size, bool hex = false);
    static std::string formatPacket(std::vector<uint8_t> payload, bool hex = false);
    static void formatPacketSDSend(std::vector<uint8_t> &payload);
protected:

    static std::string _errorMessage;

private:

    mts::ATSerial& _serial;
    static mts::ATSerial* _serialp;

    static CommandTerminal::RadioEvent* _events;
    mDot::Mode _mode;

    bool _sleep_standby;
    DigitalOut _xbee_on_sleep;
    bool _autoOTAEnabled;

    void serialLoop();
    bool autoJoinCheck();

    void printHelp();

    static bool readable();
    static bool writeable();
    static char read();
    static void write(const char* message);
    static void writef(const char* format, ... );

    void sleep(bool standby);
    void wakeup(void);

    uint8_t* _payload;
    uint8_t _size;
    uint8_t _port;
    std::vector<uint8_t> data;
};

#endif // __command_terminal_H__
