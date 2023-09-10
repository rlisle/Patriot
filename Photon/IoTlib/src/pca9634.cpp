//
//  pca9634.cpp
//  Patriot
//
//  Created by Ron Lisle on 8/24/23.
//

#include "IoT.h"

int8_t PCA9634::address;

int PCA9634::initialize(int address) {
    int status;
    int  retries;
    
    address = address; // 0x20 = no jumpers

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    Wire.beginTransmission(address);   // Seems unnecessary
    status = Wire.endTransmission();

    if(status == 0) {
        Wire.beginTransmission(address);
        Wire.write(0);          // Control register - No AI, point to reg0 Mode1
        Wire.write(0);          // Mode1 reg. Osc on, disable AI, subaddrs, allcall
        Wire.endTransmission();

        Wire.beginTransmission(address);
        Wire.write(1);          // Mode2 register
        Wire.write(0x04);       // Dimming, Not inverted, totem-pole
        Wire.endTransmission();

        Wire.beginTransmission(address);
        Wire.write(0x8c);       // AI + LEDOUT0
        Wire.write(0xaa);       // LEDOUT0 LEDs 0-3 dimming
        Wire.write(0xaa);       // LEDOUT1 LEDS 4-7 dimming
        Wire.endTransmission();

        Log.info("PCA9634 Initialize address " + String(address) + " sucess");
        
    } else {
        Log.error("PCA9634 Initialize " + String(address) + " FAILED!");
    }

    return status;
}

void PCA9634::reset() {
    Log.error("Resetting PCA9634");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

//    // Issue PCA9634 SWRST
//    Wire.beginTransmission(address);
//    Wire.write(0x06);
//    Wire.write(0xa5);
//    Wire.write(0x5a);
//    byte status = Wire.endTransmission();
//    if(status != 0){
//        Log.error("resetPCA9634 reset write failed");
//    }
    initialize(address);
}

// 8 bit PWM level 0-255
void PCA9634::ouputPWM(int lightNum, int level) {
    int reg = 2 + lightNum;
    
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(level);
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0) {
        Log.error("PCA9634 outputPWM write failed for light "+String(lightNum)+", level = "+String(level));
        reset();
    }
}
