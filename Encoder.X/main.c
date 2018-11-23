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

#define D_STEP 0.02992431454

#define WB 18.2

#define TX_BUF_SIZE 32

//#define x pos.f[0]
//#define y pos.f[1]
//#define ang pos.f[2]
//#define posChars pos.c

double x = 0;
double y = 0;
double ang = 0;

char rx_byte = 0x0;
long El = 0;
long Er = 0;

long prevEl = 0;
long prevEr = 0;

// Union to convert position data from 3 doubles to chars
//union Pose{
//   double f[3];
//   char c[9];
//}pos;  

double dDl = 0;
double dDr = 0;

double R = 0;
double dw = 0;

int tx_ind = 0;
int tx_size = 0;

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
        LED1 = (El&0x0040)<<10;
        LED2 = (Er&0x0040)<<10;
        
        GIE=0;
        if (abs(El-prevEl)>20 || abs(Er-prevEr)>20){
            // If encoder position changed, calculated incremental distance
            dDr = (double) (Er - prevEr);
            dDr = dDr*D_STEP;
            dDl = (double) (El - prevEl);
            dDl = dDl*D_STEP;
            
            R = (dDr+dDl)/2;
            dw = (dDr-dDl)/WB;
            
            x = x + R*cos( ang + dw/2 );
            y = y + R*sin( ang + dw/2 );
            ang = ang + dw;
            prevEl = El;
            prevEr = Er;
            
        }
        GIE=1;
        
    }
    
    
}

void blinky(){
    for (int i=0; i<10; ++i){
        LED1 = 1;
        LED2 = 1;
        __delay_ms(500);
        
        LED1 = 0;
        LED2 = 0;
        __delay_ms(500);
    }
}

void __interrupt() isr(void){
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
                Er--;
            }else{
                Er++;
            }
            
        }
        if (IntM2EB){
            IntM2EB=0;
            if(M2EA){
                Er++;
            }else{
                Er--;
            }
        }
        
    }
    else if(PIR3bits.TX1IF){
        // Serial Send interrupt
        if (UARTSendNext()){
            PIE3bits.TX1IE = 0;
        }
    }
    else if(PIR3bits.RC1IF){
        // Serial receive interrupt
        // Load the input buffer to be looked at
        rx_byte = RC1REG;
        
        
        
        
        if (rx_byte == 'a'){
            // If received command 'a', transmit location estimate
            int length = 0;
            // Each double is 24 bit, 3 byte
            
            tx_size = 9;
            tx_ind = 0;
            
            PIE3bits.TX1IE = 1;
            UARTSendNext();
            
        } else if(rx_byte == 'b'){
            // If received command 'b', clear location estimate
            x = 0;
            y = 0;
            ang = 0;
            TX1REG = '@';
          
        }
        else {
            // If received action is not handled, send it back through serial
            TX1REG = rx_byte;
        }
    }
    else{
       // Unhandeled interrupt, make LED blink
        blinky();
    }
}
