#ifndef NMTMASTER_H
#define	NMTMASTER_H

/*********************************************************************
 * Function:        void NMTMaster_Transmit(unsigned char nodeid, unsigned char state);
 *
 * PreCondition:    All nodes including the master must be in preoperational
 *                  state.
 *
 * Input:           nodeid Id of the node to send to a specific node or 0
 *                         to send to all nodes.
 *                  state  The state the node should be put in.
 *                         0x01 => Operational state
 *
 * Output:          none
 *
 * Side Effects:    All slaves are put into operational state.
 *
 * Overview:        Put all slaves in operational state.
 *
 * Note:
 ********************************************************************/
void NMTMaster_Transmit_NMT(unsigned char nodeid, unsigned char state);

/*********************************************************************
 * Function:        Master_GotoOperState()
 *
 * PreCondition:    Master must be in preoperational state.
 *
 * Input:           none
 *
 * Output:          none
 *
 * Side Effects:    Master is put in operational state.
 *
 * Overview:        Put master in operational state.
 *
 * Note:            COMM_STATE_PREOP cannot be set to 0 for the CANopen
 *                  library to work. That is why the function
 *                  mNMT_GotoOperState in CO_NMT.h is not used.
 ********************************************************************/
#define NMTMaster_GotoOperState() {COMM_STATE_PREOP = 1; COMM_STATE_OPER = 1; COMM_STATE_STOP = 0;}


void NMTMaster_Transmit_SDO_Heartbeat(unsigned char nodeid);

void NMTMaster_Transmit_SDO(
        unsigned char nodeid,
        unsigned char byte0,
        unsigned char byte1,
        unsigned char byte2,
        unsigned char byte3,
        unsigned char byte4,
        unsigned char byte5,
        unsigned char byte6,
        unsigned char byte7
        );

#endif	/* NMTMASTER_H */

