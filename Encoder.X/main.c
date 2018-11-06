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

// Custom Includes
#include "UART.h"

// Set Input / Output pins
#define M1EA PORTAbits.RA4
#define M1EB PORTAbits.RA5
#define M2EA PORTCbits.RC2
#define M2EB PORTCbits.RC3

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
}

void main() {
   
    init();
    return;
}
