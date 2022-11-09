#include "CmdNetworkAddress.h"
#include <algorithm>

CmdNetworkAddress::CmdNetworkAddress() :
#if MTS_CMD_TERM_VERBOSE
    Command("Network Address", "AT+NA", "Network address (devAddr in LoraMac) (4 bytes)", "(hex:4) or (1-8),(hex:4)")
#else
    Command("AT+NA")
#endif
{
    _queryable = true;
}

uint32_t CmdNetworkAddress::action(const std::vector<std::string>& args)
{
    std::vector<uint8_t> addr;
    int args_size = args.size();
    if (args_size == 1)
    {
        addr = CommandTerminal::Dot()->getNetworkAddress();
        CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
    }
    else if (args_size <= 3)
    {
        int index = 0;
        int arg_index = 1;
        if (args_size == 3) {
            arg_index = 2;
            index = args[1][0] - '0';
        }

        if ((args_size == 3) && (args[2][0] == '?')) {
            uint32_t addr_int = CommandTerminal::Dot()->getMulticastAddress(index-1);
            CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr_int >> 24, (addr_int >> 16) & 0xff, (addr_int >> 8) & 0xff, (addr_int) & 0xff);
        } else {
            int temp;
            uint32_t step = 2;
            readByteArray(args[arg_index], addr, 4);

            if (arg_index == 2 && CommandTerminal::Dot()->setMulticastAddress(index-1, addr[0] << 24 | addr[1] << 16 | addr[2] << 8 | addr[3]) == mDot::MDOT_OK) {

#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::Serial()->writef("Set Multicast Address: ");
                CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif
            } else if (arg_index == 1 && CommandTerminal::Dot()->setNetworkAddress(addr) == mDot::MDOT_OK) {

#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::Serial()->writef("Set Network Address: ");
                CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif
            } else {
                return 1;
            }
        }
    }

    return 0;
}

bool CmdNetworkAddress::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1) {
        return true;
    } else if (args.size() == 2) {

        if (!isHexString(args[1], 4))
        {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid address, expects (hex:4)");
#endif
            return false;
        }

        return true;
    } else if (args.size() == 3) {
        if (args[1].size() == 1 && (args[1][0] < '1' || args[1][0] > '8')) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid index, expects 1-8");
#endif
            return false;
        } else if (!(args[2].size() == 1 && args[2][0] == '?') && !isHexString(args[2], 4)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid address, expects (hex:4)");
#endif
            return false;
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
