#include "CmdNetworkAddress.h"
#include <algorithm>

CmdNetworkAddress::CmdNetworkAddress() :
        Command("Network Address", "AT+NA",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Network address (devAddr in LoraMac) (4 bytes)",
#else
    "",
#endif
    "(hex:4) or (1-8),(hex:4)")
{
    _queryable = true;
}

uint32_t CmdNetworkAddress::action(const std::vector<std::string>& args)
{
    std::vector<uint8_t> addr;

    if (args.size() == 1)
    {
        addr = CommandTerminal::Dot()->getNetworkAddress();
        CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
    }
    else if (args.size() == 2 || args.size() == 3)
    {
        int index = 0;
        int arg_index = 1;
        if (args.size() == 3) {
            arg_index = 2;
            sscanf(args[1].c_str(), "%d", &index);
        }

        if (args[2] == "?") {
            uint32_t addr = CommandTerminal::Dot()->getMulticastAddress(index-1);
            CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr >> 24, (addr >> 16) & 0xff, (addr >> 8) & 0xff, (addr) & 0xff);
        } else {
            int temp;
            uint32_t step = 2;

            if (args[arg_index].find(":") != std::string::npos
                || args[arg_index].find(".") != std::string::npos
                || args[arg_index].find("-") != std::string::npos)
                step = 3;

            // Convert the ASCII hex data to binary...
            for (size_t i = 0; i < args[arg_index].size(); i += step)
            {
                sscanf(&args[arg_index][i], "%02x", &temp);
                addr.push_back(temp);
            }

            if (arg_index == 2 && CommandTerminal::Dot()->setMulticastAddress(index-1, addr[0] << 24 | addr[1] << 16 | addr[2] << 8 | addr[3]) == mDot::MDOT_OK) {
                CommandTerminal::Serial()->writef("Set Network Address: ");
                CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
            } else if (arg_index == 1 && CommandTerminal::Dot()->setNetworkAddress(addr) == mDot::MDOT_OK) {
                CommandTerminal::Serial()->writef("Set Network Address: ");
                CommandTerminal::Serial()->writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
            } else {
                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
    }

    if (args.size() == 2) {

        if (!isHexString(args[1], 4))
        {
            CommandTerminal::setErrorMessage("Invalid address, expects (hex:4)");
            return false;
        }

        return true;
    }

    if (args.size() == 3) {
        int index = 0;
        sscanf(args[1].c_str(), "%d", &index);

        if (args[1].size() == 1 && (index < 1 || index > 8)) {
            CommandTerminal::setErrorMessage("Invalid index, expects 1-8");
            return false;
        } else if (args[2] != "?" && !isHexString(args[2], 4)) {
            CommandTerminal::setErrorMessage("Invalid address, expects (hex:4)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
