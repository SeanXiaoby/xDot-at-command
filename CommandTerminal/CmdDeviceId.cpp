#include "CmdDeviceId.h"
#include <algorithm>

CmdDeviceId::CmdDeviceId() :
#if MTS_CMD_TERM_VERBOSE
    Command("Device ID", "AT+DI", "Device EUI-64 (MSB) (unique, set at factory) (8 bytes)", "(hex:8)")
#else
    Command("AT+DI")
#endif
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

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid id, expects (hex:8)");
#endif

    return false;
}
