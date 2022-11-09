#include "CmdPing.h"

CmdPing::CmdPing() : Command("Send Ping", "AT+PING",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Sends ping and displays the servers received rssi and snr",
#else
    "",
#endif
    "(-140-0),(-20.0-20.0)")
{
}

uint32_t CmdPing::action(const std::vector<std::string>& args)
{
    mDot::ping_response response;
    response = CommandTerminal::Dot()->ping();
    if (response.status != mDot::MDOT_OK) {
        std::string error = mDot::getReturnCodeString(response.status);

        if (response.status != mDot::MDOT_NOT_JOINED)
            error +=  + " - " + CommandTerminal::Dot()->getLastError();

        CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
        return 1;
    }

    CommandTerminal::Serial()->writef("%d,%d.%d\r\n", response.rssi, response.snr / 10, (int)abs(response.snr) % 10);

    return 0;
}

