#include "CmdIdentification.h"
#include "mts_at_version.h"

CmdIdentification::CmdIdentification() : Command("Request Id", "ATI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Request Identification",
#else
    "",
#endif
    "NONE")
{
}

uint32_t CmdIdentification::action(const std::vector<std::string>& args)
{
    std::string version = AT_APPLICATION_VERSION;
#ifdef DEBUG_MAC
    version += "-debug";
#endif

    char buf[16];
    size_t size = snprintf(buf, sizeof(buf), "-mbed%d", MBED_VERSION);
    if (size > 0) {
        version.append(buf, size);
    }

#if defined(TARGET_MTS_MDOT_F411RE)
    CommandTerminal::Serial()->writef("MultiTech mDot\r\n");
#else
    CommandTerminal::Serial()->writef("MultiTech xDot\r\n");
#endif /* TARGET_MTS_MDOT_F411RE */
    CommandTerminal::Serial()->writef("Firmware : %s\r\n", version.c_str());
    CommandTerminal::Serial()->writef("Library  : %s\r\n", CommandTerminal::Dot()->getId().c_str());
    CommandTerminal::Serial()->writef("MTS-Lora : %s\r\n", CommandTerminal::Dot()->getMtsLoraId().c_str());

    return 0;
}
