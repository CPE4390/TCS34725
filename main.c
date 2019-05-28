#include <xc.h>
#include "LCD.h"
#include "TCS34725.h"
#include "platform.h"

#pragma config FOSC=HSPLL
#pragma config WDTEN=OFF
#pragma config XINST=OFF

void InitPins(void);
void ConfigInterrupts(void);

ColorData colorData;

void main(void) {
    OSCTUNEbits.PLLEN = 1;
    InitPins();
    LCDInit();
    lprintf(0, "TCS34725 Demo");
    InitTCS34725();
    char id = i2cReadRegister(TCS_ID);
    lprintf(1, "Chip ID = %02x", id);
    __delay_ms(1000);
    ConfigInterrupts();
    StartTCS34725(1, 100, 0, TCS_GAIN_60X);
    while (1) {
        ReadColorData(&colorData);
        lprintf(0, "C=%u R=%u", colorData.clearData, colorData.redData);
        lprintf(1, "G=%u B=%u", colorData.greenData, colorData.blueData);
        __delay_ms(500);
    }
}

void InitPins(void) {
    LATD = 0; //Turn off all LED's
    TRISD = 0; //LED's are outputs
    //Set TRIS bits for any required peripherals here.
    TRISB = 0b00000001; //Button0 is input;
    INTCON2bits.RBPU = 0; //enable weak pullups on port B
}

void ConfigInterrupts(void) {

    RCONbits.IPEN = 0; //no priorities.
    //set up INT0 to interrupt on falling edge
    INTCON2bits.INTEDG0 = 0; //interrupt on falling edge
    INTCONbits.INT0IE = 1; //Enable the interrupt
    INTCONbits.INT0IF = 0; //Always clear the flag before enabling interrupts
    INTCONbits.GIE = 1; //Turn on interrupts
}

void __interrupt(high_priority) HighIsr(void) {
    //Check the source of the interrupt
    if (INTCONbits.INT0IF == 1) {
        
        INTCONbits.INT0IF = 0; //must clear the flag to avoid recursive interrupts
    }
}
