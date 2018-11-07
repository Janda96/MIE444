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
    
void UARTInit(const uint32_t baud_rate, const uint8_t BRGH);

void UARTSendByte(const char c);

char UARTReadByte();

char UARTDataReady();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

