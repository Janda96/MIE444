
#include <xc.h>
#include "UART.h"
#include"Config.h"


extern int tx_ind;
extern int tx_size;
extern char txBuf[32];

void UARTInit(const uint32_t baud_rate) 
{
    // Set up direction of RX/TX pins
    UART_TRIS_RX = 1;
    UART_TRIS_TX = 0;
    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
    RX1DTPPSbits.RX1DTPPS = 0x15;   //RC4->EUSART1:RX1;
    RC4PPS = 0x0F;   //RC5->EUSART1:TX1;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
    // Set Baud rate
    TX1STAbits.BRGH = 1;
    BAUD1CONbits.BRG16 = 1;
    //SPBRG = _XTAL_FREQ/(64*baud_rate) - 1;
    SP1BRGH  = 0x03;
    SP1BRGL  = 0x40;
    
    // TXSTA register
    TX1STAbits.TX9 = 0;      // 8-bit transmission
    TX1STAbits.TXEN = 1;     // Enable transmission
    TX1STAbits.SYNC = 0;     // Asynchronous mode
    // Enable Serial
    RC1STAbits.SPEN = 1;     // Enable serial port
    
    // RCSTA register
    RC1STAbits.RX9 = 0;      // 8-bit reception
    RC1STAbits.CREN = 1;     // Enable continuous reception
    RC1STAbits.FERR = 0;     // Disable framing error
    RC1STAbits.OERR = 0;     // Disable overrun error
    
    UART_TRIS_TX = 0;
    TX1REG = '1';
    TX1REG = '2';
    TX1REG = '3';
    TX1REG = '4';
    TX1REG = '5';
    TX1REG = '6';
    TX1REG = '7';
    TX1REG = '8';
    TX1REG = '9';
    
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
    if(tx_ind >= tx_size){
        TX1REG = '!';
        return 1;
    }else{
    TX1REG = txBuf[tx_ind];
    ++tx_ind;
    return 0;
    }
}

char UARTReadByte() 
{
    // Wait for data to be ready
    while (!UARTDataReady());   // Wait for data to be available
    
    // Return values 
    return RC1REG;
}