#include "CmdDisplayStats.h"

CmdDisplayStats::CmdDisplayStats() :
  Command("Display Stats", "AT&S",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Display statistics",
#else
    "",
#endif
  "TABLE") {

}

uint32_t CmdDisplayStats::action(const std::vector<std::string>& args) {

    mDot::mdot_stats stats = CommandTerminal::Dot()->getStats();

    CommandTerminal::Serial()->writef("Join Attempts:  %5lu\r\n", stats.Joins);
    CommandTerminal::Serial()->writef("Join Fails:     %5lu\r\n", stats.JoinFails);
    CommandTerminal::Serial()->writef("Up Packets:     %5lu\r\n", stats.Up);
    CommandTerminal::Serial()->writef("Down Packets:   %5lu\r\n", stats.Down);
    CommandTerminal::Serial()->writef("Missed Acks:    %5lu\r\n", stats.MissedAcks);
    CommandTerminal::Serial()->writef("CRC Errors:     %5lu\r\n", stats.CRCErrors);

    return 0;
}

