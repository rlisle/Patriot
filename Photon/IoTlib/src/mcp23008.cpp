//
//  mcp23008.cpp
//  Patriot
//
// MCP23008 Registers
// 0 I/O Direction - 0 = output, 1 = input (default 0xff all inputs)
// 1 Input polarity - 1 = inverted
// 6 Pull up resistor - 1 = 100k Pull-up resistor enabled
// 9 GPIO

//  Created by Ron Lisle on 8/27/23.
//

#include "IoT.h"

int8_t MCP23008::_address;
int8_t MCP23008::_iomap;

// For I2CIO4R4G5LE board this s/b 0xf0 if all i/o's are inputs
int MCP23008::initialize(int address, int iomap) {
    int  status;
    int  retries;
    
    _address = address; // 0x20 = no jumpers
    _iomap = iomap;     // 1's = input

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x00);           // Select IO Direction register
        Wire.write(_iomap);          // 0 output, 1 input
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Set IODIR failed");
        return status;
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x06);               // Select pull-up resistor register
        Wire.write(_iomap);              // pull-ups enabled on all inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Set GPPU failed");
        return status;
    }

    retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x01);               // Polarity register
        Wire.write(_iomap);             // Invert all inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 set polarity failed");
        return status;
    }

    Log.info("MCP23008 initialize success");
    return 0;
}

int MCP23008::reset() {
    Log.error("MCP23008 Resetting");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

//    // Issue PCA9634 SWRST
//    Wire.beginTransmission(_address);
//    Wire.write(0x06);
//    Wire.write(0xa5);
//    Wire.write(0x5a);
//    byte status = Wire.endTransmission();
//    if(status != 0){
//        Log.error("MCP23008 reset write failed");
//    }
    return initialize(_address, _iomap);
}

int MCP23008::read() {
    int retries = 0;
    int status;
    
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("MCP23008 Error selecting GPIO register");
        return status;
    }
    
    Wire.requestFrom(_address, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return(data);    // Inverted ?
    }
    Log.error("MCP23008 Error reading switch");
    return 0;
}

int MCP23008::write(int ioNum, bool value) {
    // Add or remove bit from current _value
    int bitmap = 0x01 << ioNum;
    if(value==true) {   // Set bit
        value |= bitmap;
    } else {
        value &= (0xff ^ bitmap);
    }
    
    int retryCount = 3;
    int status;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x09);   // GPIO register
        
        Wire.write(value);
        
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0) {
        Log.error("MCP23008 write failed, value = %x", value);
        reset();
    }
    return status;
}
