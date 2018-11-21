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
#include <stdlib.h>
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
#define PEIE INTCONbits.PEIE

#define D_STEP 0.00928197829

#define WB 0.1524

char rx_byte = 0x0;
char tx_bytes[14];
long El = 0x0000;
long Er = 0x0000;

long prevEl = 0;
long prevEr = 0;

double x = 0;
double y = 0;
double ang = 0;

double dDl = 0;
double dDr = 0;

double R = 0;
double dw = 0;

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
    
    ADCON0 = 0b11110000;
    
    //Set interrupt masks
    PIR0 = 0x00;
    PIR3 = 0x00;
    PIE0bits.IOCIE = 1;
    PIE3bits.RC1IE = 1;
    PIE3bits.TX1IE = 1;
    
    IOCAP = 0b00110000;
    IOCCP = 0b00001100;
    
    PEIE = 1;
    //
}

void main()
{
    GIE=0;
    init();
    UARTInit(9600);
    
    
    GIE=1;
    while(1){
        LED1 = 0x0040 == (El&0x0040);
        LED2 = 0x0040 == (Er&0x0040);
        
        GIE=0;
        if (El != prevEl || Er != prevEr){
            // If encoder position changed, calculated incremental distance
            dDr = (double) (Er - prevEr);
            dDr = dDr*D_STEP;
            dDl = (double) (El - prevEl);
            dDl = dDl*D_STEP;
            
            R = (dDr+dDl)/2;
            dw = (dDr-dDl)/WB;
            
            x = x + R*cos( ang + dw/2 );
            y = y + R*cos( ang + dw/2 );
            ang = ang + dw;
        }
        GIE=1;
        
        
    }
    
    
}

void __interrupt(high_priority) isr(void){
    if (PIR0bits.IOCIF){
        // One of the encoder pin tripped
        if (IntM1EA){
            IntM1EA=0;
            if(M1EB){
                El++;
            }else{
                El--;
            }
        }
        if (IntM1EB){
            IntM1EB=0;
            if(M1EA){
                El--;
            }else{
                El++;
            }
        }
        if (IntM2EA){
            IntM2EA=0;
            IntM2EA=0;
            if(M2EB){
                Er++;
            }else{
                Er--;
            }
            
        }
        if (IntM2EB){
            IntM2EB=0;
            if(M2EA){
                Er--;
            }else{
                Er++;
            }
        }
        
    }
    else if(PIR3bits.TX1IF){
        // Serial Send interrupt
        
    }
    else if(PIR3bits.RC1IF){
        // Serial receive interrupt
        // Load the input buffer to be looked at
        rx_byte = RC1REG;
        TX1REG = rx_byte;
    }
    else{
        // Interrupt not handeled properlty
        LED1 = 1;
        LED2 = 1;
        __delay_ms(500);
        
        LED1 = 0;
        LED2 = 0;
        __delay_ms(500);
        
        LED1 = 1;
        LED2 = 1;
        __delay_ms(500);
        
        LED1 = 0;
        LED2 = 0;
        __delay_ms(500);
        LED1 = 1;
        LED2 = 1;
        __delay_ms(500);
        
        LED1 = 0;
        LED2 = 0;
        __delay_ms(500);
    }
}