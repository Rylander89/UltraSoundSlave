#include	"CO_MAIN.H"
#include        "Compilation.h"
#include        "NMTMaster.h"
#include        "Master.h"
#include        "IOutils.h"

#ifdef MASTER

#define	RTR_DIS	bytes.B1.bits.b2
#define STD_DIS	bytes.B1.bits.b3
#define PDO_DIS	bytes.B1.bits.b4


// These are mapping constants for TPDO1
// starting at 0x1A00 in the dictionary
rom unsigned long uTPDO1Map = 0x60000108;
rom unsigned long uRPDO1Map = 0x62000108;
rom unsigned long uPDO1Dummy = 0x00000008;

// Static data refered to by the dictionary
rom unsigned char rMaxIndex1 = 1;
rom unsigned char rMaxIndex2 = 8;
rom unsigned char uDemoTPDO1Len = 2;

unsigned char uIOinFilter;				// 0x6003 filter
unsigned char uIOinPolarity;				// 0x6002 polarity
unsigned char uIOinIntChange;				// 0x6006 interrupt on change
unsigned char uIOinIntRise;				// 0x6007 interrupt on positive edge
unsigned char uIOinIntFall;				// 0x6008 interrupt on negative edge
unsigned char uIOinIntEnable;				// 0x6005 enable interrupts

unsigned char uIOinDigiInOld;				// 

unsigned char uLocalXmtBuffer[8];			// Local buffer for TPDO1
unsigned char uLocalXmtBuffer2[8];			// Local buffer for TPDO2
unsigned char uLocalRcvBuffer[8];			// local buffer fot RPDO1


UNSIGNED8 uDemoState; 					// Bits used to control various states
unsigned char uDemoSyncCount;                           // Counter for synchronous types
unsigned char uDemoSyncSet;				// Internal TPDO type control


unsigned char nmtstate = 0x01;
int firstExecution = 1;
int pressedtimes = 0;
int flag1 = 0;
int flag2 = 0;


void Master_Init(void)
{
        uLocalRcvBuffer[1] = uLocalXmtBuffer[0] = 0x00;
	uLocalRcvBuffer[1] = uLocalXmtBuffer[1] = 0x00;
	uLocalRcvBuffer[2] = uLocalXmtBuffer[2] = 0x00;
	uLocalRcvBuffer[3] = uLocalXmtBuffer[3] = 0x00;
	uLocalRcvBuffer[4] = uLocalXmtBuffer[4] = 0x00;
	uLocalRcvBuffer[5] = uLocalXmtBuffer[5] = 0x00;
	uLocalRcvBuffer[6] = uLocalXmtBuffer[6] = 0x00;
	uLocalRcvBuffer[7] = uLocalXmtBuffer[7] = 0x00;

        uLocalXmtBuffer2[0] = 0x00;
        uLocalXmtBuffer2[1] = 0x01;
        uLocalXmtBuffer2[2] = 0x00;
        uLocalXmtBuffer2[3] = 0x01;
        uLocalXmtBuffer2[4] = 0x00;
        uLocalXmtBuffer2[5] = 0x01;
        uLocalXmtBuffer2[6] = 0x02;
        uLocalXmtBuffer2[7] = 0x00;

        // Convert to MCHP
//	mTOOLS_CO2MCHP(0x05 + 0xC0000280L);
//
//	// Store the COB
//	mRPDOSetCOB(1, mTOOLS_GetCOBID());

	// Convert to MCHP
	mTOOLS_CO2MCHP(0x05 + 0xC0000200L);

	// Store the COB
	mTPDOSetCOB(1, mTOOLS_GetCOBID());

        // Convert to MCHP
	mTOOLS_CO2MCHP(0x05 + 0xC0000300L);

	// Store the COB
	mTPDOSetCOB(2, mTOOLS_GetCOBID());

	// Set the pointer to the buffers
	mTPDOSetTxPtr(1, (unsigned char *)(&uLocalXmtBuffer[0]));

        // Set the pointer to the buffers
	mTPDOSetTxPtr(2, (unsigned char *)(&uLocalXmtBuffer2[0]));

//	// Set the pointer to the buffers
//	mRPDOSetRxPtr(1, (unsigned char *)(&uLocalRcvBuffer[0]));

	// Set the length
	mTPDOSetLen(1, 8);
        mTPDOSetLen(2, 8);
}

void Master_ProcessEvents(void)
{
        //Före eller efter slavar gått till OPER_STATE?
        if(firstExecution) {
                NMTMaster_GotoOperState();
                /*
                STOPLED  = COMM_STATE_STOP;
                PREOPLED = !COMM_STATE_PREOP;
                OPLED    = COMM_STATE_OPER;
                */
                firstExecution = 0;
                NMTMaster_Transmit_NMT(0x00, nmtstate);
                NMTMaster_Transmit_SDO( 0x605, 0x22, 0x14, 0x14, 0x01, 0x00, 0x00, 0x02, 0x05 ); //Enable rPDO21
                Delay10KTCYx(0x0F);
                NMTMaster_Transmit_SDO( 0x605, 0x22, 0x14, 0x15, 0x01, 0x00, 0x00, 0x03, 0x05 ); //Enable rPDO22
                Delay10KTCYx(0x0F);
                NMTMaster_Transmit_SDO( 0x605, 0x22, 0x18, 0x15, 0x01, 0x00, 0x00, 0x02, 0x85 ); //Enable tPDO22
                Delay10KTCYx(0x0F);
                NMTMaster_Transmit_SDO( 0x605, 0x2B, 0x10, 0x17, 0x00, 0x00, 0x00, 0x00, 0x0F ); //Heartbeat motor
                Delay10KTCYx(0x0F);
                mTPDOWritten(1);
                mTPDOWritten(2);
        }
        if(ButtonPressed()) {
            pressedtimes++;
        }
        if((pressedtimes == 1) && (flag1 == 0))
        {
            flag1 = 1;
            uLocalXmtBuffer2[6] = 0x02;
            uLocalXmtBuffer[0] = 0x00;
            uLocalXmtBuffer[1] = 0x20;
            mTPDOWritten(1);
            mTPDOWritten(2);
        }

        if((pressedtimes == 2) && (flag2 == 0))
        {
            flag2 = 1;
            uLocalXmtBuffer[0] = 0x00;
            uLocalXmtBuffer[1] = 0x00;
            mTPDOWritten(1);
        }

        if((pressedtimes > 3))
        {
            flag1 = 0;
            flag2 = 0;
            pressedtimes = 0;
            uLocalXmtBuffer2[6] = 0x00;
            mTPDOWritten(2);
        }

	// If any data has been received
	if (mRPDOIsGetRdy(1))
	{
                if(uLocalRcvBuffer[0] == 0xAB)
                    LED ^= 1;
		// PDO read, free the driver to accept more data
		mRPDORead(1);
	}
}










/*********************************************************************
 * Function:        void CO_COMMSyncEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        This is a simple demonstration of a SYNC event
 *					handling function.
 *
 * Note:
 ********************************************************************/
void CO_COMMSyncEvent(void)
{
	// Process only if in a synchronous mode
	if ((uDemoSyncSet == 0) && (uDemoState.bits.b2))
	{
		// Reset the synchronous transmit and transfer to async
		uDemoState.bits.b2 = 0;
		uDemoState.bits.b0 = 1;
	}
	else
	if ((uDemoSyncSet >= 1) && (uDemoSyncSet <= 240))
	{
		// Adjust the sync counter
		uDemoSyncCount--;

		// If time to generate sync
		if (uDemoSyncCount == 0)
		{
			// Reset the sync counter
			uDemoSyncCount = uDemoSyncSet;

			// Start the PDO transmission
			uDemoState.bits.b0 = 1;
		}
	}
}




/*********************************************************************
 * Function:        void CO_COMM_RPDO1_COBIDAccessEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        This is a simple demonstration of a RPDO COB access
 *					handling function.
 *
 * Note:          	This function is called from the dictionary.
 ********************************************************************/
void CO_COMM_RPDO1_COBIDAccessEvent(void)
{
	switch (mCO_DictGetCmd())
	{
		case DICT_OBJ_READ: 	// Read the object
			// Translate MCHP COB to CANopen COB
			mTOOLS_MCHP2CO(mRPDOGetCOB(1));

			// Return the COBID
			*(unsigned long *)(uDict.obj->pReqBuf) = mTOOLS_GetCOBID();
			break;

		case DICT_OBJ_WRITE: 	// Write the object
			// Translate the COB to MCHP format
			mTOOLS_CO2MCHP(*(unsigned long *)(uDict.obj->pReqBuf));

			// If the request is to stop the PDO
			if ((*(UNSIGNED32 *)(&mTOOLS_GetCOBID())).PDO_DIS)
			{
				// And if the COB received matches the stored COB and type then close
				if (!((mTOOLS_GetCOBID() ^ mRPDOGetCOB(1)) & 0xFFFFEFFFL))
				{
					// but only close if the PDO endpoint was open
					if (mRPDOIsOpen(1)) {mRPDOClose(1);}

					// Indicate to the local object that this PDO is disabled
					(*(UNSIGNED32 *)(&mRPDOGetCOB(1))).PDO_DIS = 1;
				}
				else {mCO_DictSetRet(E_PARAM_RANGE);} //error
			}

			// Else if the TPDO is not open then start the TPDO
			else
			{
				// And if the COB received matches the stored COB and type then open
				if (!((mTOOLS_GetCOBID() ^ mRPDOGetCOB(1)) & 0xFFFFEFFFL))
				{
					// but only open if the PDO endpoint was closed
					if (!mRPDOIsOpen(1)) {mRPDOOpen(1);}

					// Indicate to the local object that this PDO is enabled
					(*(UNSIGNED32 *)(&mRPDOGetCOB(1))).PDO_DIS = 0;
				}
				else {mCO_DictSetRet(E_PARAM_RANGE);} //error
			}
			break;
	}
}



/*********************************************************************
 * Function:        void CO_COMM_TPDO1_COBIDAccessEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        This is a simple demonstration of a TPDO COB access
 *					handling function.
 *
 * Note:          	This function is called from the dictionary.
 ********************************************************************/
void CO_COMM_TPDO1_COBIDAccessEvent(void)
{
	switch (mCO_DictGetCmd())
	{
		case DICT_OBJ_READ: 	// Read the object
			// Translate MCHP COB to CANopen COB
			mTOOLS_MCHP2CO(mTPDOGetCOB(1));

			// Return the COBID
			*(unsigned long *)(uDict.obj->pReqBuf) = mTOOLS_GetCOBID();
			break;

		case DICT_OBJ_WRITE: 	// Write the object
			// Translate the COB to MCHP format
			mTOOLS_CO2MCHP(*(unsigned long *)(uDict.obj->pReqBuf));

			// If the request is to stop the PDO
			if ((*(UNSIGNED32 *)(&mTOOLS_GetCOBID())).PDO_DIS)
			{
				// And if the COB received matches the stored COB and type then close
				if (!((mTOOLS_GetCOBID() ^ mTPDOGetCOB(1)) & 0xFFFFEFFFL))
				{
					// but only close if the PDO endpoint was open
					if (mTPDOIsOpen(1)) {mTPDOClose(1);}

					// Indicate to the local object that this PDO is disabled
					(*(UNSIGNED32 *)(&mTPDOGetCOB(1))).PDO_DIS = 1;
				}
				else {mCO_DictSetRet(E_PARAM_RANGE);} //error
			}

			// Else if the TPDO is not open then start the TPDO
			else
			{
				// And if the COB received matches the stored COB and type then open
				if (!((mTOOLS_GetCOBID() ^ mTPDOGetCOB(1)) & 0xFFFFEFFFL))
				{
					// but only open if the PDO endpoint was closed
					if (!mTPDOIsOpen(1)) {mTPDOOpen(1);}

					// Indicate to the local object that this PDO is enabled
					(*(UNSIGNED32 *)(&mTPDOGetCOB(1))).PDO_DIS = 0;
				}
				else {mCO_DictSetRet(E_PARAM_RANGE);} //error
			}
			break;
	}
}



/*********************************************************************
 * Function:        void CO_COMM_TPDO1_TypeAccessEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        This is a simple demonstration of a TPDO type access
 *					handling function.
 *
 * Note:          	This function is called from the dictionary.
 ********************************************************************/
void CO_COMM_TPDO1_TypeAccessEvent(void)
{
	unsigned char tempType;

	switch (mCO_DictGetCmd())
	{
		//case DICT_OBJ_INFO:		// Get information about the object
			// The application should use this to load the
			// structure with legth, access, and mapping.
		//	break;

		case DICT_OBJ_READ: 	// Read the object
			// Write the Type to the buffer
			*(uDict.obj->pReqBuf) = uDemoSyncSet;
			break;

		case DICT_OBJ_WRITE: 	// Write the object
			tempType = *(uDict.obj->pReqBuf);
			if ((tempType >= 0) && (tempType <= 240))
			{
				// Set the new type and resync
				uDemoSyncCount = uDemoSyncSet = tempType;
			}
			else
			if ((tempType == 254) || (tempType == 255))
			{
				uDemoSyncSet = tempType;
			}
			else {mCO_DictSetRet(E_PARAM_RANGE);} //error

			break;
	}
}








/*********************************************************************
 * Function:        void CO_PDO1LSTimerEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        none
 *
 * Note:          	none
 ********************************************************************/
void CO_PDO1LSTimerEvent(void)
{

}

void CO_PDO2LSTimerEvent(void)
{

}

void CO_PDO3LSTimerEvent(void)
{

}

void CO_PDO4LSTimerEvent(void)
{

}

/*********************************************************************
 * Function:        void CO_PDO1TXFinEvent(void)
 *
 * PreCondition:    none
 *
 * Input:       	none
 *
 * Output:         	none
 *
 * Side Effects:    none
 *
 * Overview:        none
 *
 * Note:          	none
 ********************************************************************/
void CO_PDO1TXFinEvent(void)
{

}

void CO_PDO2TXFinEvent(void)
{

}

void CO_PDO3TXFinEvent(void)
{

}

void CO_PDO4TXFinEvent(void)
{

}

#endif

