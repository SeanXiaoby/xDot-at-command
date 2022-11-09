#include "CmdDisplayConfig.h"

CmdDisplayConfig::CmdDisplayConfig()
:
  Command("Display Settings", "AT&V",
#if defined(TARGET_MTS_MDOT_F411RE)
  "Displays current settings and status",
#else
    "",
#endif
    "TABLE") {

}

uint32_t CmdDisplayConfig::action(const std::vector<std::string>& args) {
    CommandTerminal::Serial()->writef("Device ID:\t\t");
    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getDeviceId(), ":").c_str());

    CommandTerminal::Serial()->writef("Default Frequency Band:\t%s\r\n", CommandTerminal::Dot()->FrequencyBandStr(CommandTerminal::Dot()->getDefaultFrequencyBand()).c_str());
    // using getChannelPlanName here instead of mDot::FrequencyBandStr allows AT firmware to properly display custom channel plan names
    CommandTerminal::Serial()->writef("Current Frequency Band:\t%s\r\n", CommandTerminal::Dot()->getChannelPlanName().c_str());
    CommandTerminal::Serial()->writef("Frequency Sub Band:\t%u\r\n", CommandTerminal::Dot()->getFrequencySubBand());

    int32_t ret = CommandTerminal::Dot()->getPublicNetwork();
    string network_mode = "Not defined";
    switch(ret) {
        case 0: network_mode = "Private MTS";
                break;
        case 1: network_mode = "Public LoRaWAN";
                break;
        case 2: network_mode = "Private LoRaWAN";
                break;
    }
    CommandTerminal::Serial()->writef("Network Mode:\t\t%s\r\n", network_mode.c_str());

    CommandTerminal::Serial()->writef("Start Up Mode:\t\t%s\r\n", mDot::ModeStr(CommandTerminal::Dot()->getStartUpMode()).c_str());

    CommandTerminal::Serial()->writef("Network Address:\t%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkAddress()).c_str());

    CommandTerminal::Serial()->writef("Network ID:\t\t");
    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkId(), ":").c_str());

    CommandTerminal::Serial()->writef("Network ID Passphrase:\t%s\r\n", CommandTerminal::Dot()->getNetworkName().c_str());

    CommandTerminal::Serial()->writef("Network Key:\t\t");

    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkKey(), ".").c_str());
    CommandTerminal::Serial()->writef("Network Key Passphrase:\t%s\r\n", CommandTerminal::Dot()->getNetworkPassphrase().c_str());


    CommandTerminal::Serial()->writef("Gen App Key:\t\t");
    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getGenAppKey(), ".").c_str());

    CommandTerminal::Serial()->writef("Network Session Key:\t");
    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkSessionKey(), ".").c_str());

    CommandTerminal::Serial()->writef("Data Session Key:\t");
    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getDataSessionKey(), ".").c_str());

    CommandTerminal::Serial()->writef("Network Join Mode:\t%s\r\n", mDot::JoinModeStr(CommandTerminal::Dot()->getJoinMode()).c_str());

    CommandTerminal::Serial()->writef("Network Join Retries:\t%u\r\n", CommandTerminal::Dot()->getJoinRetries());

    CommandTerminal::Serial()->writef("Preserve Session:\t%s\r\n", CommandTerminal::Dot()->getPreserveSession() ? "on" : "off");

    CommandTerminal::Serial()->writef("Dev Nonce:\t\t%d\r\n", CommandTerminal::Dot()->getDevNonce());
    CommandTerminal::Serial()->writef("Join Nonce:\t\t%d\r\n", CommandTerminal::Dot()->getAppNonce());
    CommandTerminal::Serial()->writef("Join Delay:\t\t%d\r\n", CommandTerminal::Dot()->getJoinDelay());

    CommandTerminal::Serial()->writef("Join Rx1 DR Offset:\t%d\r\n", CommandTerminal::Dot()->getJoinRx1DataRateOffset());

    CommandTerminal::Serial()->writef("Join Rx2 Datarate:\tDR%d - %s\r\n", CommandTerminal::Dot()->getJoinRx2DataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getJoinRx2DataRate()).c_str());

    CommandTerminal::Serial()->writef("Join Rx2 Frequency:\t%lu\r\n", CommandTerminal::Dot()->getJoinRx2Frequency());

    CommandTerminal::Serial()->writef("App Port:\t\t%d\r\n", CommandTerminal::Dot()->getAppPort());

    CommandTerminal::Serial()->writef("Listen Before Talk:\t");
    if (CommandTerminal::Dot()->getLbtTimeUs() == 0 && CommandTerminal::Dot()->getLbtThreshold() == 0) {
        CommandTerminal::Serial()->writef("off\r\n");
    } else {
        CommandTerminal::Serial()->writef("%u us, %d dBm\r\n", CommandTerminal::Dot()->getLbtTimeUs(), CommandTerminal::Dot()->getLbtThreshold());
    }

    CommandTerminal::Serial()->writef("Link Check Threshold:\t");
    if (CommandTerminal::Dot()->getLinkCheckThreshold() == 0) {
        CommandTerminal::Serial()->writef("off\r\n");
    } else {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getLinkCheckThreshold());
    }

    CommandTerminal::Serial()->writef("Link Check Count:\t");
    if (CommandTerminal::Dot()->getLinkCheckCount() == 0) {
        CommandTerminal::Serial()->writef("off\r\n");
    } else {
        CommandTerminal::Serial()->writef("%lu packets\r\n", CommandTerminal::Dot()->getLinkCheckCount());
    }

    CommandTerminal::Serial()->writef("ACK Retries:\t\t");
    if (CommandTerminal::Dot()->getAck() == 0) {
        CommandTerminal::Serial()->writef("off\r\n");
    } else {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getAck());
    }

    CommandTerminal::Serial()->writef("Packet Repeat:\t\t%d\r\n", CommandTerminal::Dot()->getRepeat());

    CommandTerminal::Serial()->writef("Encryption:\t\t%s\r\n", CommandTerminal::Dot()->getAesEncryption() ? "on" : "off");
    CommandTerminal::Serial()->writef("CRC:\t\t\t%s\r\n", CommandTerminal::Dot()->getCrc() ? "on" : "off");
    CommandTerminal::Serial()->writef("Adaptive Data Rate:\t%s\r\n", CommandTerminal::Dot()->getAdr() ? "on" : "off");
    CommandTerminal::Serial()->writef("Command Echo:\t\t%s\r\n", CommandTerminal::Dot()->getEcho() ? "on" : "off");
    CommandTerminal::Serial()->writef("Verbose Response:\t%s\r\n", CommandTerminal::Dot()->getVerbose() ? "on" : "off");

    CommandTerminal::Serial()->writef("Tx Frequency:\t\t%lu\r\n", CommandTerminal::Dot()->getTxFrequency());

    CommandTerminal::Serial()->writef("Tx Data Rate:\t\tDR%d - %s\r\n", CommandTerminal::Dot()->getTxDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getTxDataRate()).c_str());
    CommandTerminal::Serial()->writef("Min/Max Tx Data Rate:\tMin: DR%d - %s\r\n\t\t\tMax: DR%d - %s\r\n", CommandTerminal::Dot()->getMinDatarate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getMinDatarate()).c_str(), CommandTerminal::Dot()->getMaxDatarate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getMaxDatarate()).c_str());
    CommandTerminal::Serial()->writef("Tx Power:\t\t%u\r\n", CommandTerminal::Dot()->getTxPower());
    CommandTerminal::Serial()->writef("Min/Max Tx Power:\tMin: %u\r\n\t\t\tMax: %u\r\n", CommandTerminal::Dot()->getMinTxPower(), CommandTerminal::Dot()->getMaxTxPower());
    CommandTerminal::Serial()->writef("Tx Antenna Gain:\t%d\r\n", CommandTerminal::Dot()->getAntennaGain());
    CommandTerminal::Serial()->writef("Tx Wait:\t\t%s\r\n", CommandTerminal::Dot()->getTxWait() ? "on" : "off");

    bool hasChannels = false;
    for (size_t i = 0; i < 16; i++) {
        if (CommandTerminal::Dot()->getConfigChannels()[i] != 0) {
            hasChannels = true;
            break;
        }
    }

    if (hasChannels) {
        CommandTerminal::Serial()->writef("Tx Channels --------------------------------------\r\n");
        CommandTerminal::Serial()->writef("\tIndex\tFrequency\tDR Range\r\n");
        for (size_t i = 0; i < 16; i++) {
            if (CommandTerminal::Dot()->getConfigChannels()[i] != 0)
                CommandTerminal::Serial()->writef("\t%d:\t%09lu\t%02x\r\n", i, CommandTerminal::Dot()->getConfigChannels()[i], CommandTerminal::Dot()->getConfigChannelRanges()[i]);
        }
        CommandTerminal::Serial()->writef("--------------------------------------------------\r\n");
    }

    CommandTerminal::Serial()->writef("Rx Delay:\t\t%d s\r\n", CommandTerminal::Dot()->getRxDelay());
//    CommandTerminal::Serial()->writef("Rx Frequency:\t\t%lu\r\n", CommandTerminal::Dot()->getRxFrequency());
//    CommandTerminal::Serial()->writef("Rx Data Rate:\t\t%s\r\n", mDot::DataRateStr(CommandTerminal::Dot()->getRxDataRate()).c_str());

    CommandTerminal::Serial()->writef("Rx Output Style:\t%s\r\n", mDot::RxOutputStr(CommandTerminal::Dot()->getRxOutput()).c_str());

    CommandTerminal::Serial()->writef("Debug Baud Rate:\t%lu\r\n", CommandTerminal::Dot()->getDebugBaud());
    CommandTerminal::Serial()->writef("Serial Baud Rate:\t%lu\r\n", CommandTerminal::Dot()->getBaud());
    CommandTerminal::Serial()->writef("Serial Flow Control:\t%s\r\n", CommandTerminal::Dot()->getFlowControl() == 0 ? "off" : "on");
    CommandTerminal::Serial()->writef("Serial Clear On Error:\t%s\r\n", CommandTerminal::Dot()->getSerialClearOnError() == 0 ? "off" : "on");

    uint8_t mode = CommandTerminal::Dot()->getWakeMode();
    CommandTerminal::Serial()->writef("Wake Mode:\t\t%s\r\n", mode == 0 ? "INTERVAL" : ( mode == 1 ? "INTERRUPT" : "BOTH"));
    CommandTerminal::Serial()->writef("Wake Interval:\t\t%lu s\r\n", CommandTerminal::Dot()->getWakeInterval());
    CommandTerminal::Serial()->writef("Wake Delay:\t\t%lu ms\r\n", CommandTerminal::Dot()->getWakeDelay());
    CommandTerminal::Serial()->writef("Wake Timeout:\t\t%u ms\r\n", CommandTerminal::Dot()->getWakeTimeout());
    CommandTerminal::Serial()->writef("Wake Pin:\t\t%s\r\n", mDot::pinName2Str(CommandTerminal::Dot()->getWakePin()).c_str());

    CommandTerminal::Serial()->writef("Log Level:\t\t%ld\r\n", CommandTerminal::Dot()->getLogLevel());

    return 0;
}

