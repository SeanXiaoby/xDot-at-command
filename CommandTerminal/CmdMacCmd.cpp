#include "CmdMacCmd.h"
#include "CommandTerminal.h"

CmdMacCmd::CmdMacCmd()
:
  Command("MAC Command", "AT+MAC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Inject MAC command to MAC layer or read uplink MAC command buffer, pass '0' argument to clear buffer",
#else
    "",
#endif
    "(hex:242)")
{
}

uint32_t CmdMacCmd::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        std::vector<uint8_t> cmds = CommandTerminal::Dot()->getMacCommands();

        if (cmds.size() > 0) {
            for (size_t i = 0; i < cmds.size(); i++) {
                CommandTerminal::Serial()->writef("%02x", cmds[i]);
            }
            CommandTerminal::Serial()->writef("\r\n");
        }

    } else {
        if (args[1].size() == 1 && args[1] == "0") {
            CommandTerminal::Dot()->clearMacCommands();
        } else {
            std::vector<uint8_t> data;
            int32_t code;
            int temp;
            uint32_t length = args[1].size();

            // Convert the ASCII hex data to binary...
            for (uint32_t i = 0; i < length; i += 2) {
                sscanf(&args[1][i], "%2x", &temp);
                data.push_back(temp);
            }

            if ((code = CommandTerminal::Dot()->injectMacCommand(data)) != mDot::MDOT_OK) {
                std::string error = mDot::getReturnCodeString(code);

                if (code != mDot::MDOT_NOT_JOINED)
                    error += +" - " + CommandTerminal::Dot()->getLastError();

                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
                return 1;
            }
        }
    }

    return 0;
}

bool CmdMacCmd::verify(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        return true;
    } else if (args.size() == 2) {

        if (args[1].size() == 1 && args[1] == "0") {
            return true;
        } else if (args[1].size() > 484 || !isHexString(args[1], args[1].size() / 2)) {
            CommandTerminal::setErrorMessage("Invalid hex string, (hex:242)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

