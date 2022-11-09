#include "mbed.h"
#include "mDot.h"
#include "CommandTerminal.h"
#include "ChannelPlans.h"

#if defined(TARGET_XDOT_L151CC) && defined(FOTA)
#include "SPIFBlockDevice.h"
#include "DataFlashBlockDevice.h"
#endif

mbed::UnbufferedSerial debug_port(USBTX, USBRX, LOG_DEFAULT_BAUD_RATE);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &debug_port;
}


#if defined(TARGET_MTS_MDOT_F411RE)
#define AT_TX_PIN   XBEE_DOUT
#define AT_RX_PIN   XBEE_DIN
#define AT_RTS_PIN  XBEE_RTS
#define AT_CTS_PIN  XBEE_CTS
#elif defined(TARGET_XDOT_L151CC)
#define AT_TX_PIN   UART1_TX
#define AT_RX_PIN   UART1_RX
#define AT_RTS_PIN  UART1_RTS
#define AT_CTS_PIN  UART1_CTS
#else
#error Unsupported target
#endif

#ifndef CHANNEL_PLAN
#define CHANNEL_PLAN CP_US915
#endif

#ifndef UNIT_TEST
#ifndef UNIT_TEST_MDOT_LIB

DigitalIn boot(PA_5);

int main()
{
#if defined(DEBUG_MAC) && defined(TARGET_MTS_MDOT_F411RE)
    lora::ChannelPlan* plan = new lora::ChannelPlan_US915();
    mDot* dot = mDot::getInstance(plan);
#else
#if CHANNEL_PLAN == CP_AS923
    lora::ChannelPlan_AS923 plan;
#elif CHANNEL_PLAN == CP_AS923_2
    lora::ChannelPlan_AS923 plan;
#elif CHANNEL_PLAN == CP_AS923_3
    lora::ChannelPlan_AS923 plan;
#elif CHANNEL_PLAN == CP_US915
    lora::ChannelPlan_US915 plan;
#elif CHANNEL_PLAN == CP_AU915
    lora::ChannelPlan_AU915 plan;
#elif CHANNEL_PLAN == CP_EU868
    lora::ChannelPlan_EU868 plan;
#elif CHANNEL_PLAN == CP_KR920
    lora::ChannelPlan_KR920 plan;
#elif CHANNEL_PLAN == CP_IN865
    lora::ChannelPlan_IN865 plan;
#elif CHANNEL_PLAN == CP_AS923_JAPAN
    lora::ChannelPlan_AS923_Japan plan;
#elif CHANNEL_PLAN == CP_AS923_JAPAN1
    lora::ChannelPlan_AS923_Japan1 plan;
#elif CHANNEL_PLAN == CP_AS923_JAPAN2
    lora::ChannelPlan_AS923_Japan2 plan;
#elif CHANNEL_PLAN == CP_RU864
    lora::ChannelPlan_RU864 plan;
#endif
#if defined(TARGET_XDOT_L151CC) && defined(FOTA)

    mbed::BlockDevice* ext_bd = NULL;

    ext_bd = new SPIFBlockDevice();
    int ret = ext_bd->init();
    if (ext_bd->init() < 0) {
        delete ext_bd;
        ext_bd = new DataFlashBlockDevice();
        ret = ext_bd->init();
        // Check for zero size because DataFlashBlockDevice doesn't
        // return an error if the chip is not present
        if ((ret < 0) || (ext_bd->size() == 0)) {
            delete ext_bd;
            ext_bd = NULL;
        }
    }

    mDot* dot = mDot::getInstance(&plan, ext_bd);

    if (ext_bd != NULL) {
        logInfo("External flash device detected, type: %s, size: 0x%08x",
            ext_bd->get_type(), (uint32_t)ext_bd->size());
    }
#else
    mDot* dot = mDot::getInstance(&plan);
#endif
#endif //MTS_MDOT_F411RE
    assert(dot);

#if defined(DEBUG_MAC) && defined(TARGET_MTS_MDOT_F411RE)
    logDebug("Loading default channel plan %02x", dot->getDefaultFrequencyBand());
    switch (dot->getDefaultFrequencyBand()) {
        case lora::ChannelPlan::AS923:
        case lora::ChannelPlan::AS923_2:
        case lora::ChannelPlan::AS923_3:
            delete plan;
            plan = new lora::ChannelPlan_AS923();
            break;

        case lora::ChannelPlan::US915:
        case lora::ChannelPlan::US915_OLD:
            delete plan;
            plan = new lora::ChannelPlan_US915();
            break;

        case lora::ChannelPlan::AU915:
        case lora::ChannelPlan::AU915_OLD:
            delete plan;
            plan = new lora::ChannelPlan_AU915();
            break;

        case lora::ChannelPlan::EU868:
        case lora::ChannelPlan::EU868_OLD:
            delete plan;
            plan = new lora::ChannelPlan_EU868();
            break;

        case lora::ChannelPlan::KR920:
            delete plan;
            plan = new lora::ChannelPlan_KR920();
            break;

        case lora::ChannelPlan::AS923_JAPAN:
            delete plan;
            plan = new lora::ChannelPlan_AS923_Japan();
            break;

        case lora::ChannelPlan::AS923_JAPAN1:
            delete plan;
            plan = new lora::ChannelPlan_AS923_Japan1();
            break;

        case lora::ChannelPlan::AS923_JAPAN2:
            delete plan;
            plan = new lora::ChannelPlan_AS923_Japan2();
            break;

        case lora::ChannelPlan::IN865:
            delete plan;
            plan = new lora::ChannelPlan_IN865();
            break;

        case lora::ChannelPlan::RU864:
            delete plan;
            plan = new lora::ChannelPlan_RU864();
            break;

        default:
            logInfo("Default channel plan not valid Defaulting US915");
            delete plan;
            plan = new lora::ChannelPlan_US915();
            break;
    }
    dot->setChannelPlan(plan);
#endif
    // Seed the RNG
    srand(dot->getRadioRandom());


    PinName rtsPin = NC;
    PinName ctsPin = NC;
    if (dot->getFlowControl()) {
        rtsPin = AT_RTS_PIN;
        ctsPin = AT_CTS_PIN;
    }

    mts::ATSerial serial(AT_TX_PIN, AT_RX_PIN, rtsPin, ctsPin, dot->getBaud());

    debug_port.baud(dot->getDebugBaud());

    CommandTerminal term(serial);
    CommandTerminal::_dot = dot;

    Fota::getInstance(dot);

    term.init();

    term.start();
}

#endif // UNIT_TEST
#endif
