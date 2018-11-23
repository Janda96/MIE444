
#include <xc.h>
#include "UART.h"
#include"Config.h"


extern int tx_ind;
extern int tx_size;
//extern union Pose{
//   double f[3];
//   char c[9];
//}pos;

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
    RC1STAbits.RX9 = 0;      // 8-bit reception
    RC1STAbits.CREN = 1;     // Enable continuous reception
    RC1STAbits.FERR = 0;     // Disable framing error
    RC1STAbits.OERR = 0;     // Disable overrun error
    
    // Enable Serial
    RC1STAbits.SPEN = 1;     // Enable serial port
    
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

char UARTSendNext() 
{
    // Set data in transmit register
    TX1REG = 0x00;
    ++tx_ind;
    return (tx_ind >= tx_size);
}

char UARTReadByte() 
{
    // Wait for data to be ready
    while (!UARTDataReady());   // Wait for data to be available
    
    // Return values 
    return RC1REG;
}