#include "CmdPing.h"

CmdPing::CmdPing() :
#if MTS_CMD_TERM_VERBOSE
    Command("Send Ping", "AT+PING", "Sends ping and displays the servers received rssi and snr", "(-140-0),(-20.0-20.0)")
#else
    Command("AT+PING")
#endif
{
}

uint32_t CmdPing::action(const std::vector<std::string>& args)
{
    mDot::ping_response response;
    response = CommandTerminal::Dot()->ping();
    if (response.status != mDot::MDOT_OK) {
        // std::string error = mDot::getReturnCodeString(response.status);

        // if (response.status != mDot::MDOT_NOT_JOINED)
            // error +=  + " - " + CommandTerminal::Dot()->getLastError();
            // CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
        return 1;
    }

    CommandTerminal::Serial()->writef("%d,%d.%d\r\n", response.rssi, response.snr / 10, (int)abs(response.snr) % 10);

    return 0;
}

