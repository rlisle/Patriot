//
//  mcp23008.cpp
//  Patriot
//
//  Created by Ron Lisle on 8/27/23.
//

#include "IoT.h"

int8_t MCP23008::address;

void MCP23008::initialize(int address, int iomap) {
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
        Wire.write(iomap);                   // 1 output, 0 input
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Set IODIR failed");
        return;
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xff ^ iomap);           // pull-ups enabled on all inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Set GPPU failed");
    }
}

void MCP23008::reset() {
    Log.error("MCP23008 Resetting PCA9634");
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
        Log.error("MCP23008 resetPCA9634 reset write failed");
    }
    initialize(address, 0xf0);
}

int MCP23008::read() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Error selecting GPIO register");
        return false;
    }
    
    Wire.requestFrom(address, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return(data);    // Inverted ?
    }
    Log.error("MCP23008 Error reading switch");
    return 0;
}

void MCP23008::write(int ioNum, bool value) {
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x09);   // GPIO register
        
        //TODO: Need to insert bit into existing mask
        Wire.write(0xff);   // All bits HIGH
        
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0) {
        Log.error("MCP23008 write failed");
        reset();
    }
}
