//
//  pca9634.cpp
//  Patriot
//
//  Created by Ron Lisle on 8/24/23.
//

#include "IoT.h"

void PCA9685::initialize(int addr) {
    byte status;
    int  retries;
    
    address = addr;         // 0x41 = lowest jumper

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(254);    // PRE_SCALE for PWM output freq
        Wire.write(5);      // 5 = ?
        Wire.endTransmission();
        
        Wire.beginTransmission(address);
        Wire.write(0);      // MODE1
        Wire.write(161);    // 128=Restart Enabled, internal clock, 32=AutoIncrement, 1=AllCall
        // MODE2 left default: NotInverted, Output on Stop command, OpenDrain, OUTNE
        status = Wire.endTransmission();

    } while( status != 0 && retries++ < 3);
    
    if(status == 0) {
        Log.info("PCA9685 InitializeBoard sucess");
    } else {
        Log.error("PCA9685 InitializeBoard FAILED!");
    }
}

void PCA9685::reset() {
    Log.error("Resetting PCA9685");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9685 SWRST - this doesn't look correct, so commented out for now
//    Wire.beginTransmission(_address);
//    Wire.write(0x06);       // Isn't this LED0???
//    Wire.write(0xa5);
//    Wire.write(0x5a);
//    byte status = Wire.endTransmission();
//    if(status != 0){
//        Log.error("NCD16Light reset write failed for light "+String(_lightNum)+", reseting Wire");
//    }
    initialize(address);
}

void PCA9685::outputPWM(int lightNum, int level) {
    int reg = 6 + (lightNum * 4); // 0 based, not 1 based like older plugins

    int lsb = level & 0xff;
    int msb = level >> 8;
    
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(0);      // Delay lsb
        Wire.write(0);      // Delay msb
        Wire.write(lsb);    // Off lsb
        Wire.write(msb);    // Off msb
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0) {
        Log.error("NCD16Light outputPWM write failed for light "+String(lightNum)+", level = "+String(level));
        reset();
    }
}
