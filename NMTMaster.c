#include	"CO_MAIN.H"
#include        "NMTMaster.h"

void NMTMaster_Transmit_NMT(unsigned char nodeid, unsigned char state)
{
    mCANIsPutReady(0);
    *(unsigned long *)(mCANGetPtrTxCOB()) = 0; // Sets both RXB0SIDH and RXB0SIDL to 0.
    mCANPutDataLen(2); // NMT-messages are 2 bytes long(state+nodeid).
    mCANPutDataByte0(state); 
    mCANPutDataByte1(nodeid);
    mCANSendMessage();
}

void NMTMaster_Transmit_SDO_Heartbeat(unsigned char nodeid)
{
    mCANIsPutReady(0);

    mTOOLS_CO2MCHP(nodeid + 0xC0000600L);
    *(unsigned long *)(mCANGetPtrTxCOB()) = mTOOLS_GetCOBID();

    mCANPutDataLen(8); 

    mCANPutDataByte0(0x2B); 
    mCANPutDataByte1(0x17); 
    mCANPutDataByte2(0x10); 
    mCANPutDataByte3(0x00); 
    mCANPutDataByte4(0xA0);
    mCANPutDataByte5(0x0F);
    mCANPutDataByte6(0x00);
    mCANPutDataByte7(0x00);

    mCANSendMessage();
}

void NMTMaster_Transmit_SDO(
        unsigned char nodeid,
        unsigned char cmd,
        unsigned char index1,
        unsigned char index2,
        unsigned char subindex,
        unsigned char databyte0,
        unsigned char databyte1,
        unsigned char databyte2,
        unsigned char databyte3
        )
{
    mCANIsPutReady(0);

    mTOOLS_CO2MCHP(nodeid + 0xC0000600L);
    *(unsigned long *)(mCANGetPtrTxCOB()) = mTOOLS_GetCOBID();

    mCANPutDataLen(8);

    mCANPutDataByte0(cmd); //0x22); // 0010_0010
    mCANPutDataByte1(index2); //index
    mCANPutDataByte2(index1); //index
    mCANPutDataByte3(subindex); //subindex
    mCANPutDataByte4(databyte3); //data...
    mCANPutDataByte5(databyte2);
    mCANPutDataByte6(databyte1);
    mCANPutDataByte7(databyte0);

    mCANSendMessage();
}

