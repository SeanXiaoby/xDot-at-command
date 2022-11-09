#include "CommandFactory.h"

#if MTS_CMD_TERM_VERBOSE
#define CMD_DUMMY(n,t,d,u)  CmdDummy(n,t,d,u)
#else
#define CMD_DUMMY(n,t,d,u)  CmdDummy(t)
#endif

Command* CommandFactory::Create(CmdId_t cmd)
{
    switch (cmd) {
    case eAT:
        return new CMD_DUMMY("Attention", "AT", "Attention", "");
    case eATI:
        return new CmdIdentification();
    case eLW:
        return new CMD_DUMMY("LoRaWAN MAC Version", "AT+LW", "Show support LoRaWAN MAC Version", "");
    case eATZ:
        return new CmdResetCpu();
    case eATE:
        return new CMD_DUMMY("Enable/Disable Echo", "ATE", "ATE0: disable, ATE1: enable", "(0,1)");
    case eATVERBOSE:
        return new CMD_DUMMY("Enable/Disable Verbose", "ATV", "ATV0: disable, ATV1: enable", "(0,1)");
    case eATK:
        return new CMD_DUMMY("Hardware Flow Control", "AT&K", "AT&K0: disable, AT&K3: enable", "(0,3)");
    case eATF:
        return new CmdFactoryDefault();
    case eATW:
        return new CMD_DUMMY("Save Configuration", "AT&W", "Save configuration to flash memory", "NONE");
#if MTS_CMD_TERM_VERBOSE
    case eATV:
        return new CmdDisplayConfig();
    case eATS:
        return new CmdDisplayStats();
#endif
    case eATR:
        return new CmdResetStats();
    case eIPR:
        return new CmdSerialBaudRate();
    case eDIPR:
        return new CmdDebugBaudRate();
    case eSMODE:
        return new CmdStartUpMode();
    case eDFREQ:
        return new CmdDefaultFrequencyBand();
    case eFREQ:
        return new CmdFrequencyBand();
    case eFSB:
        return new CmdFrequencySubBand();
    case ePN:
        return new CmdPublicNetwork();
    case eDI:
        return new CmdDeviceId();
    case eDC:
        return new CmdDeviceClass();
    case eLBT:
        return new CmdLbt();
    case eAP:
        return new CmdAppPort();
    case eNA:
        return new CmdNetworkAddress();
    case eNSK:
        return new CmdNetworkSessionKey();
    case eDSK:
        return new CmdDataSessionKey();
    case eULC:
        return new CmdUplinkCounter();
    case eDLC:
        return new CmdDownlinkCounter();
    case eSS:
        return new CMD_DUMMY("Save Network Session", "AT+SS", "Save network session info to flash", "NONE");
    case eRS:
        return new CmdRestoreSession();
    case eNK:
        return new CmdNetworkKey();
    case eGK:
        return new CmdGenAppKey();
    case eNI:
        return new CmdNetworkId();
    case eJD:
        return new CmdJoinDelay();
    case eJOIN:
        return new CmdJoinRequest();
    case eJR:
        return new CmdJoinRetries();
    case eJN:
        return new CmdJoinNonce();
    case eJNV:
        return new CmdJoinNonceValidation();
    case eNJM:
        return new CmdNetworkJoinMode();
    case ePS:
        return new CmdPreserveSession();
    case eNJS:
        return new CmdNetworkJoinStatus();
    case eNLC:
        return new CmdNetworkLinkCheck();
    case eLCC:
        return new CmdLinkCheckCount();
    case eLCT:
        return new CmdLinkCheckThreshold();
    case eRSSI:
        return new CmdRssi();
    case eSNR:
        return new CmdSnr();
    case eDP:
        return new CMD_DUMMY("Data Pending", "AT+DP", "Indicator of data in queue on server", "(0,1)");
    case eSDR:
        return new CmdSessionDataRate();
    case eCHM:
        return new CmdChannelMask();
    case eTXDR:
        return new CmdTxDataRate();
    case eTXP:
        return new CmdTxPower();
    case eANT:
        return new CmdAntennaGain();
    case eTXF:
        return new CmdTxFrequency();
    case eFO:
        return new CmdTxFrequencyOffset();
    case eTXW:
        return new CmdTxWait();
    case eTXCH:
        return new CmdTxChannel();
    case eTXN:
        return new CmdTxNextMs();
    case eTXS:
        return new CmdTxNextSize();
    case eTOA:
        return new CmdTimeOnAir();
    case eMCRX:
        return new CmdMulticastRx();
    case eRXD:
        return new CmdRxDelay();
    case eRXO:
        return new CmdRxOutput();
    case eADR:
        return new CmdAdaptiveDataRate();
    case eACK:
        return new CmdACKAttempts();
    case eBTO:
        return new CmdClassBTimeout();
    case eCTO:
        return new CmdClassCTimeout();
    case eREP:
        return new CmdRepeat();
    case ePP:
        return new CmdPingPeriod();
    case eMAC:
        return new CmdMacCmd();
    case eSEND:
        return new CmdSendString();
    case eSENDB:
        return new CmdSendBinary();
    case eRECV:
        return new CmdReceiveOnce();
    case eURC:
        return new CMD_DUMMY("Unsolicited Response Code", "AT+URC", "Output packets to terminal when received", "(0,1)");
    case eSD:
        return new CMD_DUMMY("Serial Data Mode", "AT+SD", "Enter serial data mode, exit with '+++'", "NONE");
    case eSLEEP:
        return new CMD_DUMMY("Sleep Mode", "AT+SLEEP", "Enter sleep mode (0:deepsleep,1:sleep)", "(0,1)");
    case eSDCE:
        return new CmdSerialClearOnError();
    case eWM:
        return new CmdWakeMode();
    case eWI:
        return new CmdWakeInterval();
    case eWP:
        return new CmdWakePin();
    case eWD:
        return new CmdWakeDelay();
    case eWTO:
        return new CmdWakeTimeout();
    case ePING:
        return new CmdPing();
    case eLOG:
        return new CmdLogLevel();
    case eFOTA:
        return new CmdFota();
    case eGPSTIME:
        return new CmdGpsTime();
    case eBLS:
        return new CmdBeaconLockStatus();
    case eLBTRSSI:
        return new CmdLBTRSSI();
    case eBAT:
        return new CmdBatteryLevel();
    case eATWP:
        return new CMD_DUMMY("Write Protected Config", "AT&WP", "Write protected config to flash (DevEUI, AppEUI, AppKey, Frequency Band)", "NONE");
#if defined(TARGET_MTS_MDOT_F411RE)
    case eREPAIR:
        return new CmdRepairFlash();
    case eWOTP:
        return new CmdWriteOtp();
#endif
    case eDUTY:
        return new CmdDutyCycle();
#if MTS_CMD_TERM_TEST_COMMANDS
    case ePRINT_TEST:
        return new CMD_DUMMY("***** Test Commands *****", "", "", "");
    case eRXDR:
        return new CmdRxDataRate();
    case eSENDI:
        return new CmdSendStringOnInterval();
    case eSENDC:
        return new CmdSendContinuous();
    case eMEM:
        return new CMD_DUMMY("Free Memory", "AT+MEM", "Show amount of free RAM available", "");
#endif
#ifdef MTS_RADIO_DEBUG_COMMANDS
    case ePRINT_DEBUG:
        return new CMD_DUMMY("***** Debug Commands *****", "", "", "");
    case eDREGS:
        return new CmdDumpRegisters();
    case eERASE:
        return new CmdEraseFlash();
    case eDD:
        return new CmdDisableDutyCycle();
    case eRXF:
        return new CmdRxFrequency();
    case eRECVC:
        return new CmdReceiveContinuous();
    case eRTC:
        return new CmdRtc();
#endif
    default:
        return NULL;
    }
}
