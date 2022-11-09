#include "CmdWriteOtp.h"

CmdWriteOtp::CmdWriteOtp() :
#if MTS_CMD_TERM_VERBOSE
    Command("Write One-Time-Programmable Memory", "AT+WOTP", "Write protected settings to One-Time-Programmable memory", "(1)")
#else
    Command("AT+WOTP")
#endif
{
    _queryable = true;
}

uint32_t CmdWriteOtp::action(const std::vector<std::string>& args)
{
#if defined(TARGET_MTS_MDOT_F411RE)
    int ret;
    if ((args.size() == 2) && (args[1].length() == 1 && (args[1][0] == '1'))) {
        ret = CommandTerminal::Dot()->writeOtp();
        if (ret < 0) {
            return 1;
        }

        CommandTerminal::Serial()->writef("%d\r\n", ret);
    } else {
        bool ver = CommandTerminal::Dot()->verifyOtp(&ret);
        CommandTerminal::Serial()->writef("Verified %d\r\nRemaining %d\r\n", ver, ret);
    }
#endif
    return 0;
}

bool CmdWriteOtp::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if ((args.size() == 2) && (args[1].length() == 1 && (args[1][0] == '1'))) {
        return true;
    }
#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid parameter, expects 1");
#endif
    return false;
}
