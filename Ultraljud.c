#include "Ultraljud.h"
#include <usart.h>

void Init_Ultra(void)
{
    // configure USART 129 ändras om det behövs, beroende på baudrate
    OpenUSART( USART_TX_INT_OFF &
    USART_RX_INT_OFF &
    USART_ASYNCH_MODE &
    USART_EIGHT_BIT &
    USART_CONT_RX &
    USART_BRGH_HIGH,
    129 );
}
int ReadDistance(unsigned short *dist)
{
    char temp[4];
    char *p = temp;
    if(DataRdyUSART())
    {
        getsUSART( p, 4 );
        if(*p == 'R')
        {
            *dist = 0;
            p++;
            *dist = (int)(*p - 48)*100;
            p++;
            *dist += (int)(*p - 48)*10;
            p++;
            *dist += (int)(*p - 48)*1;
            return 1;
        }
    }
    return 0;
}
