
#include <xc.h>
#include "UART.h"
#include"Config.h"

void UARTInit(const uint32_t baud_rate) 
{
    // Set Baud rate
    
    TX1STAbits.BRGH = 1;
    BAUD1CONbits.BRG16 = 1;
    SPBRG = _XTAL_FREQ/(64*baud_rate) - 1;
    
    // TXSTA register
    TX1STAbits.TX9 = 0;      // 8-bit transmission
    TX1STAbits.TXEN = 1;     // Enable transmission
    TX1STAbits.SYNC = 0;     // Asynchronous mode
    
    // RCSTA register
    RC1STAbits.SPEN = 1;     // Enable serial port
    RC1STAbits.RX9 = 0;      // 8-bit reception
    RC1STAbits.CREN = 1;     // Enable continuous reception
    RC1STAbits.FERR = 0;     // Disable framing error
    RC1STAbits.OERR = 0;     // Disable overrun error
    
    // Set up direction of RX/TX pins
    UART_TRIS_RX = 1;
    UART_TRIS_TX = 0;
}

void UARTSendByte(const char c) 
{
    // Set data in transmit register
    TX1REG = c;
    
    // Wait for data to send
    while (TX1STAbits.TRMT == 0);
}

char UARTReadByte() 
{
    // Wait for data to be ready
    while (!UARTDataReady());   // Wait for data to be available
    
    // Return values 
    return RC1REG;
}