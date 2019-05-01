/* 
 * File:   TCS34725.h
 * Author: bmcgarvey
 *
 * Created on April 30, 2019, 8:37 AM
 */

#ifndef TCS34725_H
#define	TCS34725_H

#define I2C_ADDRESS     0x29

//Command codes
#define CMD_CLEAR_INT   0b11100110
#define CMD_REPEAT_BYTE 0b10000000
#define CMD_AUTO_INC    0b10100000

//Registers
#define TCS_ENABLE          0x00
#define TCS_ATIME           0x01
#define TCS_WTIME           0x03
#define TCS_AILTL           0x04
#define TCS_AILTH           0x05
#define TCS_AIHTL           0x06
#define TCS_AIHTH           0x07
#define TCS_PERS            0x0c
#define TCS_CONFIG          0x0d
#define TCS_CONTROL         0x0f
#define TCS_ID              0x12
#define TCS_STATUS          0x13
#define TCS_CDATAL          0x14
#define TCS_CDATAH          0x15
#define TCS_RDATAL          0x16
#define TCS_RDATAH          0x17
#define TCS_GDATAL          0x18
#define TCS_GDATAH          0x19
#define TCS_BDATAL          0x1a
#define TCS_BDATAH          0x1b

//Gain values
#define TCS_GAIN_1X         0
#define TCS_GAIN_4X         1
#define TCS_GAIN_16X        2
#define TCS_GAIN_60X        3


#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct {
        unsigned int clearData;
        unsigned int redData;
        unsigned int greenData;
        unsigned int blueData;
    } ColorData;

    void InitTCS34725(void);
    void I2CWriteRegister(unsigned char reg, unsigned char byte);
    void I2CWriteRegisterWord(unsigned char reg, unsigned int word);
    void I2CReadData(unsigned char reg, unsigned char *buffer, char count);
    unsigned char I2CReadRegister(unsigned char reg);
    void ClearInt(void);
    void StartTCS34725(unsigned char integrationSteps, unsigned char waitSteps, char longTime, unsigned char gain);
    void ReadColorData(ColorData *data);

#ifdef	__cplusplus
}
#endif

#endif	/* TCS34725_H */

