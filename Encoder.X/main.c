/*
 * File:   main.c
 * Author: andrej
 *
 * Created on November 6, 2018, 2:24 PM
 */
#include "Config.h"

// Standard Includes
#include <xc.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

// Custom Includes
#include "UART.h"

// Set Input / Output pins
#define M1EA PORTAbits.RA4
#define M1EB PORTAbits.RA5
#define M2EA PORTCbits.RC2
#define M2EB PORTCbits.RC3
#define LED2 PORTAbits.RA0
#define LED1 PORTAbits.RA1

// Pre define interrupt registers
#define IntM1EA IOCAFbits.IOCAF4
#define IntM1EB IOCAFbits.IOCAF5
#define IntM2EA IOCCFbits.IOCCF2
#define IntM2EB IOCCFbits.IOCCF3

#define GIE INTCONbits.GIE

char rx_byte = 0x0;
long tx_byte = 0x0000;
long E1_count = 0x0000;
long E2_count = 0x0000;

void init()
{
    // LEDS
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;

    // Motor Encoder Pins (INPUT)
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC3 = 1;
    
    // Disable Analogue Input Buffers and Weak Pullup Resistors
    ANSELA = 0x00;
    ANSELC = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;
    
    ADCON0 = 0b1111000001;
    
    //Set interrupt masks
    PIR0 = 0x00;
    PIR3 = 0x00;
    PIE0 = 0b00010000;
    PIE3 = 0b00100000;
    
    IOCAP = 0b00110000;
    IOCCP = 0b00001100;
    
    //
}

void main()
{
    GIE=0;
    init();
    //UARTInit(9600);
    
    
    
    GIE=1;
    while(1){
        LED1 = (E1_count&0x0040)&&0x0040;
        LED2 = (E2_count&0x0040)&&0x0040;
    }
    
    
}

void __interrupt(high_priority) isr(void){
    GIE=0;
    if (PIR0bits.IOCIF){
        // One of the encoder pin tripped
        if (IntM1EA){
            IntM1EA=0;
            if(M1EB){
                E1_count++;
            }else{
                E1_count--;
            }
        }
        if (IntM1EB){
            IntM1EB=0;
            if(M1EA){
                E1_count--;
            }else{
                E1_count++;
            }
        }
        if (IntM2EA){
            IntM2EA=0;
            IntM2EA=0;
            if(M2EB){
                E2_count++;
            }else{
                E2_count--;
            }
            
        }
        if (IntM2EB){
            IntM2EB=0;
            if(M2EA){
                E2_count--;
            }else{
                E2_count++;
            }
        }
        
    }
    if(PIR3bits.RC1IF){
        // Serial receive interrupt
        // Load the input buffer to be looked at
        rx_byte = RC1REG;
        TX1REG = rx_byte;
    }
    GIE=1;
}