#ifndef __COMMANDFACTORY_H__
#define __COMMANDFACTORY_H__

#include "Commands.h"

#include <string>

class CommandFactory
{
public:
    CommandFactory() { };
    ~CommandFactory() { };

    typedef enum {
        eAT,
        eATI,
        eATZ,
        eATE,
        eATVERBOSE,
        eATK,
        eATF,
        eATW,
#if MTS_CMD_TERM_VERBOSE
        eATV,
        eATS,
#endif
        eATR,
        eIPR,
        eDIPR,
        eSMODE,
        eDFREQ,
        eFREQ,
        eFSB,
        ePN,
        eDI,
        eDC,
        eLBT,
        eAP,
        eNA,
        eNSK,
        eDSK,
        eULC,
        eDLC,
        eSS,
        eRS,
        eNK,
        eNI,
        eGK,
        eJD,
        eJOIN,
        eJR,
        eJN,
        eJNV,
        eNJM,
        ePS,
        eNJS,
        eNLC,
        eLCC,
        eLCT,
        eRSSI,
        eSNR,
        eDP,
        eSDR,
        eCHM,
        eTXDR,
        eTXP,
        eANT,
        eTXF,
        eFO,
        eTXW,
        eTXCH,
        eTXN,
        eTXS,
        eTOA,
        eMCRX,
        eRXD,
        eRXO,
        eADR,
        eACK,
        eBTO,
        eCTO,
        eREP,
        ePP,
        eMAC,
        eSEND,
        eSENDB,
        eRECV,
        eURC,
        eLW,
        eSD,
        eSLEEP,
        eSDCE,
        eWM,
        eWI,
        eWP,
        eWD,
        eWTO,
        ePING,
        eFOTA,
        eLOG,
        eGPSTIME,
        eBLS,
        eLBTRSSI,
        eBAT,
        eATWP,
#if defined(TARGET_MTS_MDOT_F411RE)
        eREPAIR,
        eWOTP,
#endif
        eDUTY,
#if MTS_CMD_TERM_TEST_COMMANDS
        ePRINT_TEST,
        eRXDR,
        eSENDI,
        eSENDC,
        eMEM,
#endif
#ifdef MTS_RADIO_DEBUG_COMMANDS
        ePRINT_DEBUG,
        eDREGS,
        eERASE,
        eDD,
        eRXF,
        eRECVC,
        eRTC,
#endif
        NUMBER_OF_CMDS
    } CmdId_t;

    static Command* Create(CmdId_t cmd);
};
#endif
