#include "CmdFactory.h"

Command* CmdFactory::Create(CmdId_t cmd)
{
    switch (cmd) {
    case eAT:
        return new CmdAttention();
    case eATI:
        return new CmdIdentification();
    case eATZ:
        return new CmdResetCpu();
    case eATE:
        return new CmdDummy("Enable/Disable Echo", "ATE", "ATE0: disable, ATE1: enable", "(0,1)");
    case eATVERBOSE:
        return new CmdDummy("Enable/Disable Verbose", "ATV", "ATV0: disable, ATV1: enable", "(0,1)");
    case eATK:
        return new CmdDummy("Hardware Flow Control", "AT&K", "AT&K0: disable, AT&K3: enable", "(0,3)");
    case eATF:
        return new CmdFactoryDefault();
    case eATW:
        return new CmdSaveConfig();
    case eATV:
        return new CmdDisplayConfig();
    case eATS:
        return new CmdDisplayStats();
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
        return new CmdSaveSession();
    case eRS:
        return new CmdRestoreSession();
    case eNK:
        return new CmdNetworkKey();
    case eNI:
        return new CmdNetworkId();
    case eJD:
        return new CmdJoinDelay();
    case eJOIN:
        return new CmdJoinRequest();
    case eJR:
        return new CmdJoinRetries();
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
    case eENC:
        return new CmdEncryption();
    case eRSSI:
        return new CmdRssi();
    case eSNR:
        return new CmdSnr();
    case eDP:
        return new CmdDataPending();
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
    case eRXD:
        return new CmdRxDelay();
    case eRXO:
        return new CmdRxOutput();
    case eCRC:
        return new CmdCRC();
    case eADR:
        return new CmdAdaptiveDataRate();
    case eACK:
        return new CmdACKAttempts();
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
    case eSD:
        return new CmdDummy("Serial Data Mode", "AT+SD", "Enter serial data mode, exit with '+++'", "NONE");
    case eSLEEP:
        return new CmdDummy("Sleep Mode", "AT+SLEEP", "Enter sleep mode (0:deepsleep,1:sleep)", "(0,1)");
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
    case ePRINT_TEST:
        return new CmdDummy("***** Test Commands *****", "", "", "");
    case eRXDR:
        return new CmdRxDataRate();
    case eSENDI:
        return new CmdSendStringOnInterval();
    case eSENDC:
        return new CmdSendContinuous();
#ifdef MTS_RADIO_DEBUG_COMMANDS
    case ePRINT_DEBUG:
        return new CmdDummy("***** Debug Commands *****", "", "", "");
    case eATWP:
        return new CmdWriteProtectedConfig();
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
#endif
    default:
        return NULL;
    }
}
