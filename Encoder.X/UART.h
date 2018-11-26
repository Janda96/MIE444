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
    
#define UART_TRIS_RX TRISCbits.TRISC5
#define UART_TRIS_TX TRISCbits.TRISC4
#define UART_RX PORTCbits.RC5
#define UART_TX PORTCbits.RC4
    
void UARTInit(const uint32_t baud_rate);

void UARTSendByte(const char c);

char UARTSendNext();

char UARTReadByte();

char UARTDataReady();


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

