#include <xc.h>
#include "TCS34725.h"
#include "platform.h"

void InitTCS34725(void) {
    initPlatform();
}

void StartTCS34725(unsigned char integrationSteps, unsigned char waitSteps, char longTime, unsigned char gain) {
    i2cWriteRegister(TCS_ATIME, -integrationSteps);
    i2cWriteRegister(TCS_WTIME, -waitSteps);
    if (longTime) {
        i2cWriteRegister(TCS_CONFIG, 0b00000010);
    } else {
        i2cWriteRegister(TCS_CONFIG, 0);
    }
    i2cWriteRegister(TCS_CONTROL, gain & 0b00000011);
    i2cWriteRegister(TCS_ENABLE, 0b00001011);
}

void ReadColorData(ColorData *data) {
    unsigned char status;
    //TODO is this necessary?
    do {
        status = i2cReadRegister(TCS_STATUS);
    } while ((status & 1) == 0);
    i2cReadData(TCS_CDATAL, (unsigned char *)data, 8);
}

