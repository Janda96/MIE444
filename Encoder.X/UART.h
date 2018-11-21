/* 
 * File:   UART.h
 * Author: andrej
 *
 * Created on November 6, 2018, 3:12 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
#define UART_TRIS_RX TRISCbits.TRISC4
#define UART_TRIS_TX TRISCbits.TRISC5
#define UART_RX PORTCbits.RC4
#define UART_TX PORTCbits.RC5
    
void UARTInit(const uint32_t baud_rate);

void UARTSendByte(const char c);

char UARTSendNext(int *tx_buf, int *tc_ind) 

char UARTReadByte();

char UARTDataReady();


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

