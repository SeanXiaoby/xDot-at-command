#include "CmdLbt.h"

CmdLbt::CmdLbt() :
    Command("Listen Before Talk", "AT+LBT",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Enable/Disable listen before talk (0,0: disable, time,threshold: enable)",
#else
    "",
#endif
    "time(0-65535 us),threshold(-128-127 dBm) (0,0: disable, time,threshold: enable)")
{
    _queryable = true;
}

uint32_t CmdLbt::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u,%d\r\n", CommandTerminal::Dot()->getLbtTimeUs(), CommandTerminal::Dot()->getLbtThreshold());
    }
    else
    {
        uint32_t us;
        int32_t rssi;

        sscanf(args[1].c_str(), "%lu", &us);
        sscanf(args[2].c_str(), "%ld", &rssi);

        CommandTerminal::Dot()->setLbtTimeUs((uint16_t)us);
        CommandTerminal::Dot()->setLbtThreshold((int8_t)rssi);
    }

    return 0;
}

bool CmdLbt::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 3) {
        uint32_t us;
        int32_t rssi;

        if (args[1].find("-") != std::string::npos || sscanf(args[1].c_str(), "%lu", &us) != 1 || us > 65535) {
            CommandTerminal::setErrorMessage("Invalid LBT time, expects 0-65535 us");
            return false;
        }

        if (sscanf(args[2].c_str(), "%ld", &rssi) != 1 || rssi < -128 || rssi > 127) {
            CommandTerminal::setErrorMessage("Invalid LBT threshold, expects -128-127 dBm");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
