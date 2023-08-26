//
//  pca9634.cpp
//  Patriot
//
//  Created by Ron Lisle on 8/24/23.
//

#include "IoT.h"

int8_t PCA9634::address;

void PCA9634::initialize(int address, int iomap) {
    byte status;
    int  retries;
    
    address = address; // 0x20 = no jumpers

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(iomap);                   // 0-3 relays, 4-7 inputs
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
        return;
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0 & iomap);           // pull-ups enabled on all inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
    }
}

void PCA9634::reset() {
    Log.error("Resetting PCA9634");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("resetPCA9634 reset write failed");
    }
    initialize(address, 0xf0);
}

bool PCA9634::isInputOn(int bitmap) {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
        return false;
    }
    
    Wire.requestFrom(address, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return((data & bitmap) == 0);    // Inverted
    }
    Log.error("Error reading switch");
    return false;
}
