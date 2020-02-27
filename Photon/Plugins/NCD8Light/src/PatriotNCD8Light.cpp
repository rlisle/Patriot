/******************************************************************
 NCD PCA9634 8-channel PWM board control

 Up to 8 I2C boards can reside on a single I2C bus.
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2020-02-11: Initial creation based on NCD8Relay
 ******************************************************************/

#include "PatriotNCD8Light.h"

#define MILLIS_PER_SECOND 1000

int8_t NCD8Light::_address;             // Addresses of board

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7) 0x01 if low switch set
 * @param lightNum is the channel number on the NCD 8 Light board (0-7)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD8Light::NCD8Light(int8_t address, int8_t lightNum, String name, int8_t duration)
    : Device(name, DeviceType::NCD8Light)
{
    _lightNum   = lightNum;
    // _percent is left uninitialized to pickup state from SRAM
    _duration   = duration;
    _address = address + 0x40;  // Expected values = 0 thru ?
    initializeBoard();
}

int8_t NCD8Light::initializeBoard() {
    int retries;
    byte status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_address);
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3); // Seems redundant

    if(status == 0) {
        Wire.beginTransmission(_address);
        Wire.write(0);          // Mode1 register
        Wire.write(0);          // Osc on, disable AI, subaddrs, allcall
        Wire.endTransmission();

        Wire.beginTransmission(_address);
        Wire.write(1);          // Mode2 register
        Wire.write(0x16);       // Invert, Outdrv
        Wire.endTransmission();

    } else {
        Serial.println("initializeBoard failed");
    }

    return status;
}

/**
 * Set percent
 * @param percent Int 0 to 100.
 */
void NCD8Light::setPercent(int percent) {
    /* TODO: implement smooth transitions  */
    /*       Refer to Light plugin example */
    //int val = 255 * percent / 100;      // Convert 0-100 to 0-255    
    int reg = 2 + _lightNum;
    Wire.beginTransmission(_address);
	Wire.write(reg);
	Wire.write(percent);
	byte status = Wire.endTransmission();
	if(status != 0){
		Serial.println("Write failed");
	}else{
		//Command successful
	}    
}


/**
 * Private Methods
 */

/**
 * loop()
 */
void NCD8Light::loop()
{
    //TODO: Implement smooth transitions
    //      Nothing to do until then

};
