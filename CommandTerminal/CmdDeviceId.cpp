#include "CmdDeviceId.h"
#include <algorithm>

CmdDeviceId::CmdDeviceId() :
        Command("Device ID", "AT+DI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Device EUI-64 (MSB) (unique, set at factory) (8 bytes)",
#else
    "",
#endif
    "(hex:8)")
{
    _queryable = true;
}

uint32_t CmdDeviceId::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getDeviceId(), "-").c_str());
    }
    else if (args.size() == 2)
    {
        std::vector<uint8_t> NewEUI;

        // Read in the key components...
        readByteArray(args[1], NewEUI, EUI_LENGTH);

        if (CommandTerminal::Dot()->setDeviceId(NewEUI) == mDot::MDOT_OK) {
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
        } else {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdDeviceId::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2 && isHexString(args[1], 8))
        return true;

    CommandTerminal::setErrorMessage("Invalid id, expects (hex:8)");

    return false;
}
