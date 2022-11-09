#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "Command.h"
#include "CmdDummy.h"

#include "CmdIdentification.h"
#include "CmdResetCpu.h"
// Echo built into command terminal
// Verbose built into command terminal
//
#include "CmdFactoryDefault.h"
#if MTS_CMD_TERM_VERBOSE
#include "CmdDisplayConfig.h"
#include "CmdDisplayStats.h"
#endif
#include "CmdResetStats.h"

#include "CmdSerialBaudRate.h"
#include "CmdSerialClearOnError.h"
#include "CmdDebugBaudRate.h"
#include "CmdStartUpMode.h"

#include "CmdDefaultFrequencyBand.h"
#include "CmdFrequencyBand.h"
#include "CmdDeviceId.h"
#include "CmdDeviceClass.h"
#include "CmdPublicNetwork.h"
#include "CmdNetworkAddress.h"
#include "CmdNetworkSessionKey.h"
#include "CmdDataSessionKey.h"
#include "CmdUplinkCounter.h"
#include "CmdDownlinkCounter.h"
#include "CmdRestoreSession.h"
#include "CmdNetworkKey.h"
#include "CmdGenAppKey.h"
#include "CmdNetworkId.h"
#include "CmdAppPort.h"
#include "CmdJoinRequest.h"
#include "CmdJoinRetries.h"
#include "CmdJoinNonce.h"
#include "CmdJoinNonceValidation.h"
#include "CmdJoinDelay.h"
// Remove join settings commands until valid case for changing default settings
//#include "CmdJoinRx1Offset.h"
//#include "CmdJoinRx2Datarate.h"
//#include "CmdJoinRx2Frequency.h"
#include "CmdNetworkJoinMode.h"
#include "CmdPreserveSession.h"
#include "CmdNetworkJoinStatus.h"
#include "CmdNetworkLinkCheck.h"
#include "CmdLinkCheckCount.h"
#include "CmdLinkCheckThreshold.h"
#include "CmdRssi.h"
#include "CmdSnr.h"

#include "CmdSessionDataRate.h"
#include "CmdChannelMask.h"

#include "CmdTxDataRate.h"
#include "CmdAntennaGain.h"
#include "CmdTxPower.h"
#include "CmdTxWait.h"
#include "CmdTxFrequencyOffset.h"
#include "CmdTxChannel.h"
#include "CmdTxNextMs.h"
#include "CmdTxNextSize.h"
#include "CmdTimeOnAir.h"
#include "CmdFrequencySubBand.h"
#include "CmdLbt.h"

#include "CmdRxDelay.h"
#include "CmdRxOutput.h"
#include "CmdMulticastRx.h"

#include "CmdAdaptiveDataRate.h"

#include "CmdACKAttempts.h"
#include "CmdRepeat.h"
#include "CmdPingPeriod.h"
#include "CmdMacCmd.h"
#include "CmdSendString.h"
#include "CmdSendBinary.h"
#include "CmdReceiveOnce.h"
#include "CmdPing.h"
#include "CmdClassBTimeout.h"
#include "CmdClassCTimeout.h"

// Serial Data Mode built into command terminal
// Sleep built into command terminal
#include "CmdWakeInterval.h"
#include "CmdWakePin.h"
#include "CmdWakeMode.h"
#include "CmdWakeDelay.h"
#include "CmdWakeTimeout.h"

#include "CmdLogLevel.h"

#include "CmdFota.h"
#include "CmdGpsTime.h"
#include "CmdBeaconLockStatus.h"
#include "CmdLBTRSSI.h"
#include "CmdBatteryLevel.h"
#include "CmdTxFrequency.h"
#include "CmdDutyCycle.h"

#if MTS_CMD_TERM_TEST_COMMANDS
#include "CmdRxDataRate.h"
#include "CmdSendStringOnInterval.h"
#include "CmdSendContinuous.h"
#endif

#if defined(TARGET_MTS_MDOT_F411RE)
#include "CmdRepairFlash.h"
#include "CmdWriteOtp.h"
#endif

#ifdef MTS_RADIO_DEBUG_COMMANDS
// Radio debug commands
#include "CmdDumpRegisters.h"
#include "CmdEraseFlash.h"
#include "CmdDisableDutyCycle.h"
#include "CmdRxFrequency.h"
#include "CmdReceiveContinuous.h"
#include "CmdRtc.h"
#endif

#endif // __COMMANDS_H__
