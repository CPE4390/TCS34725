/* 
 * File:   platform.h
 * Author: bmcgarvey
 *
 * Created on May 2, 2019, 7:27 AM
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

#ifdef	__cplusplus
extern "C" {
#endif

    void initPlatform(void);
    void i2cWriteRegister(unsigned char reg, unsigned char byte);
    void i2cWriteRegisterWord(unsigned char reg, unsigned int word);
    void i2cReadData(unsigned char reg, unsigned char *buffer, char count);
    unsigned char i2cReadRegister(unsigned char reg);
    void i2cClearInt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PLATFORM_H */

