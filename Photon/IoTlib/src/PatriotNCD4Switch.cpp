/**
 PatriotNCD4Switch

 Features:
 - Read and debounce switch input

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-01-16: Initial creation
 2022-12-10: Change MQTT message to patriot/<device>/get/position
 */

#include "IoT.h"

#define POLL_INTERVAL_MILLIS 100
#define FILTER_INCREMENT 25

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7) 0x20-0x27
 * @param switchIndex is the switch number on the NCD board (0-3)
 * @param name String name used to address the relay.
 */
NCD4Switch::NCD4Switch(int8_t boardAddress, int8_t switchIndex, String name, String room)
    : Device(name, room)
{
    _boardAddress = boardAddress;   // 0x20 (no jumpers)
    _lastPollTime = 0;
    _type         = 'S';
    _filter       = 0;
    
    if(switchIndex > 0 && switchIndex <= 3) {
        _switchBitmap = 0x10 << switchIndex;
    } else {
        _switchBitmap = 0x10;   // If 0 or invalid, set to first switch
    }
}

void NCD4Switch::begin() {

    byte status;
    int  retries;

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xf0);                   // 0-3 relays, 4-7 in
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0);                   // pull-ups enabled on inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
    }
}

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD4Switch::isSwitchOn() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
    }
    
    Wire.requestFrom(_boardAddress, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return((data & _switchBitmap) == 0);    // Inverted
    }
    Log.error("Error reading switch");
    return false;
}

void NCD4Switch::reset() {
    Log.error("Resetting board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(_boardAddress);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD4Switch reset write failed for switch bitmap: "+String(_switchBitmap)+", re-initializing board");
    }
    begin();
}



/**
 * loop()
 */
void NCD4Switch::loop()
{
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            notify();
        }
    }
}

// Private Helper Methods
/**
 * isTimeToCheckSwitch()
 * @return bool if enough time has elapsed to sample switch again
 */
bool NCD4Switch::isTimeToCheckSwitch()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}


/**
 * didSwitchChange()
 * @return bool if switch has changed since last reading
 */
bool NCD4Switch::didSwitchChange()
{
    int newValue = isSwitchOn() ? 100 : 0;
    bool oldState = (_value != 0);
    
    if(newValue == 100) {   // Is switch on?
        _filter += FILTER_INCREMENT;
        if(_filter > 100) {
            _filter = 100;
        }
    } else {    // Switch is off
        _filter -= FILTER_INCREMENT;
        if(_filter < 0) {
            _filter = 0;
        }
    }

    if(oldState == false && _filter == 100) {
        _value = 100;
        return true;
    }
    
    if(oldState == true && _filter == 0) {
        _value = 0;
        return true;
    }
    
    return false;
}


/**
 * notify()
 * Publish switch state
 */
void NCD4Switch::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}

