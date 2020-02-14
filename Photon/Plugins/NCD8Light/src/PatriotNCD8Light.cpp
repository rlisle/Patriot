/******************************************************************
 NCD 8 PWM board control

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

int8_t NCD8Light::_currentStates[8];    // Cached light states (initially off)
int8_t NCD8Light::_address;             // Addresses of board

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param lightNum is the channel number on the NCD 8 Light board (1-8)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD8Light::NCD8Light(int8_t address, int8_t lightNum, String name, int8_t duration)
    : Device(name, DeviceType::NCD8Light)
{
    _lightNum   = lightNum;
    // _percent is left uninitialized to pickup state from SRAM
    _duration   = duration;
    _address = address + 64
    initializeBoard(_address);
}

int8_t NCD8Light::initializeBoard(int8_t address) {
    int retries;
    byte status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(address);
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3); // Seems redundant

    if(status == 0) {
        Wire.beginTransmission(address);
        Wire.write(254);
        Wire.write(5);
        Wire.endTransmission();

        Wire.beginTransmission(address);
        Wire.write(0);
        Wire.write(161);
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
    int val = 409 * percent; // 4095 * percent / 100
    int reg = (_lightNum*4)+2;
    wire.beginTransmission(_address);
	Wire.write(reg);
	Wire.write(0);
	Wire.write(0);
	Wire.write(val);
	Wire.write(val>>8);
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
