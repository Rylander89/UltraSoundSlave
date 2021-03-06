#ifndef IOUTILS_H
#define	IOUTILS_H

#include <p18F4680.h>

#define BTN             PORTEbits.RE2
#define LED             PORTAbits.RA0
#define STOPLED         PORTAbits.RA1
#define PREOPLED        PORTAbits.RA2
#define OPLED           PORTAbits.RA3
#define SWITCH          PORTAbits.RA5

#define PRESSED         1

extern unsigned char buttonWasPressed;

void IO_Init(void);

unsigned char ButtonPressed(void);

#endif	/* IOUTILS_H */

