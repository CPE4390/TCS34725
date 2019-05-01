#include <xc.h>
#include "TCS34725.h"

void InitTCS34725(void) {
    TRISD = 0b01100000; //MMSP2 uses RD5 as SDA, RD6 as SCL, both set as inputs
    SSP2ADD = 0x19; //400kHz
    SSP2CON1bits.SSPM = 0b1000; //I2C Master mode
    SSP2CON1bits.SSPEN = 1; //Enable MSSP
}

void StartTCS34725(unsigned char integrationSteps, unsigned char waitSteps, char longTime, unsigned char gain) {
    I2CWriteRegister(TCS_ATIME, -integrationSteps);
    I2CWriteRegister(TCS_WTIME, -waitSteps);
    if (longTime) {
        I2CWriteRegister(TCS_CONFIG, 0b00000010);
    } else {
        I2CWriteRegister(TCS_CONFIG, 0);
    }
    I2CWriteRegister(TCS_CONTROL, gain & 0b00000011);
    I2CWriteRegister(TCS_ENABLE, 0b00001011);
}

void ReadColorData(ColorData *data) {
    unsigned char status;
    //TODO is this necessary?
    do {
        status = I2CReadRegister(TCS_STATUS);
    } while ((status & 1) == 0);
    I2CReadData(TCS_CDATAL, (unsigned char *)data, 8);
}

void I2CWriteRegister(unsigned char reg, unsigned char byte) {
    SSP2CON2bits.SEN = 1; //Start condition
    while (SSP2CON2bits.SEN == 1); //Wait for start to finish
    SSP2BUF = I2C_ADDRESS << 1; //address with R/W clear for write
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = CMD_REPEAT_BYTE | (reg & 0b00011111); //Send register and command
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = byte; //Send byte
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.PEN = 1; //Stop condition
    while (SSP2CON2bits.PEN == 1); //Wait for stop to finish
}

void I2CWriteRegisterWord(unsigned char reg, unsigned int word) {
    SSP2CON2bits.SEN = 1; //Start condition
    while (SSP2CON2bits.SEN == 1); //Wait for start to finish
    SSP2BUF = I2C_ADDRESS << 1; //address with R/W clear for write
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = CMD_AUTO_INC | (reg & 0b00011111); //Send register and command
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = word; //Send lower byte
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = word >> 8; //Send upper byte
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.PEN = 1; //Stop condition
    while (SSP2CON2bits.PEN == 1); //Wait for stop to finish
}

unsigned char I2CReadRegister(unsigned char reg) {
    unsigned char data;
    SSP2CON2bits.SEN = 1; //Start condition
    while (SSP2CON2bits.SEN == 1); //Wait for start to finish
    SSP2BUF = I2C_ADDRESS << 1; //address with R/W clear for write
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = CMD_REPEAT_BYTE | (reg & 0b00011111); //Send register and command
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.RSEN = 1; //Restart condition
    while (SSP2CON2bits.RSEN == 1); //Wait for restart to finish
    SSP2BUF = (I2C_ADDRESS << 1) + 1; //address with R/W set for read
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.RCEN = 1; // enable master for 1 byte reception
    while (!SSP2STATbits.BF); // wait until byte received
    data = SSP2BUF;
    SSP2CON2bits.ACKDT = 1;
    SSP2CON2bits.ACKEN = 1; //Send ACK/NACK
    while (SSP2CON2bits.ACKEN != 0);
    SSP2CON2bits.PEN = 1; //Stop condition
    while (SSP2CON2bits.PEN == 1); //Wait for stop to finish
    return data;
}

void I2CReadData(unsigned char reg, unsigned char *buffer, char count) {
    char i;
    SSP2CON2bits.SEN = 1; //Start condition
    while (SSP2CON2bits.SEN == 1); //Wait for start to finish
    i = SSP2BUF; //Read SSPxBUF to make sure BF is clear
    SSP2BUF = I2C_ADDRESS << 1; //address with R/W clear for write
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = CMD_AUTO_INC | (reg & 0b00011111); //Send register and command
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.RSEN = 1; //Restart condition
    while (SSP2CON2bits.RSEN == 1); //Wait for restart to finish
    SSP2BUF = (I2C_ADDRESS << 1) + 1; //address with R/W set for read
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    for (i = 0; i < count; ++i) {
        SSP2CON2bits.RCEN = 1; // enable master for 1 byte reception
        while (!SSP2STATbits.BF); // wait until byte received
        buffer[i] = SSP2BUF;
        if (i == count - 1) {
            SSP2CON2bits.ACKDT = 1;
        } else {
            SSP2CON2bits.ACKDT = 0;
        }
        SSP2CON2bits.ACKEN = 1; //Send ACK/NACK
        while (SSP2CON2bits.ACKEN != 0);
    }
    SSP2CON2bits.PEN = 1; //Stop condition
    while (SSP2CON2bits.PEN == 1); //Wait for stop to finish
}

void ClearInt(void) {
    SSP2CON2bits.SEN = 1; //Start condition
    while (SSP2CON2bits.SEN == 1); //Wait for start to finish
    SSP2BUF = I2C_ADDRESS << 1; //address with R/W clear for write
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2BUF = CMD_CLEAR_INT; //Send command
    while (SSP2STATbits.BF || SSP2STATbits.R_W); // wait until write cycle is complete
    SSP2CON2bits.PEN = 1; //Stop condition
    while (SSP2CON2bits.PEN == 1); //Wait for stop to finish
}