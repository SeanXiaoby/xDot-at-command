#include "mbed.h"
#include "mDot.h"
#include "CommandTerminal.h"
#include "ATSerial.h"
#include "ATSerialFlowControl.h"
#include "ChannelPlans.h"

#define SERIAL_BUFFER_SIZE 512

Serial debug_port(USBTX, USBRX);

#ifndef CHANNEL_PLAN
#define CHANNEL_PLAN CP_US915
#endif

#ifndef UNIT_TEST
#ifndef UNIT_TEST_MDOT_LIB
int main()
{
    debug_port.baud(115200);
#if defined(DEBUG_MAC) && defined(TARGET_MTS_MDOT_F411RE) 
    lora::ChannelPlan* plan = new lora::ChannelPlan_US915();
    mDot* dot = mDot::getInstance(plan);
#else
#if CHANNEL_PLAN == CP_AS923
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
#elif CHANNEL_PLAN == CP_RU864
    lora::ChannelPlan_RU864 plan;
#endif
    mDot* dot = mDot::getInstance(&plan);
#endif //MTS_MDOT_F411RE
    assert(dot);

#if defined(DEBUG_MAC) && defined(TARGET_MTS_MDOT_F411RE) 
    logDebug("Loading default channel plan %02x", dot->getDefaultFrequencyBand());
    switch (dot->getDefaultFrequencyBand()) {
        case lora::ChannelPlan::AS923:
            delete plan;  
            plan = new lora::ChannelPlan_AS923();
            break;

        case lora::ChannelPlan::US915_OLD:
            delete plan;
            plan = new lora::ChannelPlan_US915();
            break;

        case lora::ChannelPlan::AU915_OLD:
            delete plan;
            plan = new lora::ChannelPlan_AU915();
            break;

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

    mts::ATSerial* serial;

    if (dot->getFlowControl())
#if defined(TARGET_MTS_MDOT_F411RE)
        serial = new mts::ATSerialFlowControl(XBEE_DOUT, XBEE_DIN, XBEE_RTS, XBEE_CTS, SERIAL_BUFFER_SIZE, SERIAL_BUFFER_SIZE);
#else
        serial = new mts::ATSerialFlowControl(UART1_TX, UART1_RX, UART1_RTS, UART1_CTS, SERIAL_BUFFER_SIZE, SERIAL_BUFFER_SIZE);
#endif
    else
#if defined(TARGET_MTS_MDOT_F411RE)
        serial = new mts::ATSerial(XBEE_DOUT, XBEE_DIN, SERIAL_BUFFER_SIZE, SERIAL_BUFFER_SIZE);
#else
        serial = new mts::ATSerial(UART1_TX, UART1_RX, SERIAL_BUFFER_SIZE, SERIAL_BUFFER_SIZE);
#endif

    debug_port.baud(dot->getDebugBaud());
    serial->baud(dot->getBaud());

    CommandTerminal term(*serial);
    CommandTerminal::_dot = dot;

    Fota::getInstance(dot);

    term.init();

    term.start();
}

#endif // UNIT_TEST
#endif
