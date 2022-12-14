/**
 NCD GPIO Switch control

 Features:
 - Switch inputs converted to patriot MQTT messages
 - Supports multiple boards

 It is expected that the input will be connected to a switch
 that is connected to ground. The pull-up resistor will be
 enabled, making this a very simple circuit.
 Therefore the logic is inverted: low = 100, high = 0
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-06-20: Initial creation
 2022-12-10: Change MQTT message to patriot/<device>/get/position
 */

#include "PatriotNCD8Switch.h"
#include "IoT.h"                // Is this needed?

#define POLL_INTERVAL_MILLIS 250
#define FILTER_INCREMENT 25

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param switchIndex is the switch index on the NCD 8 GPIO board (0-7)
 * @param name String name used in MQTT messages
 */
NCD8Switch::NCD8Switch(int boardAddress, int switchIndex, String name, String room)
                : Device(name, room)
{
    _boardAddress = boardAddress;
    _lastPollTime = 0;
    _type         = 'S';
    _filter       = 0;
    
    if(switchIndex > 0 && switchIndex <= 7) {
        _switchBitmap = 0x01 << switchIndex;
    } else {
        _switchBitmap = 0x01;   // If 0 or invalid, set to first switch
    }
}

void NCD8Switch::begin() {

    if(_switchBitmap == 0) {
        Log.error("Invalid switchNum");
        return;
    }
    initializeBoard();
}

/**
 * Private Methods
 */

int NCD8Switch::initializeBoard() {
    int retries;
    byte status;
    
    // Only the first switch loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xff);                   // Set all 8 to inputs
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xff);                   // pull-ups enabled on all 8 outputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Initialize board failed");
    }
    
    return status;
}

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD8Switch::isSwitchOn() {
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

void NCD8Switch::reset() {
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
        Log.error("NCD8Switch reset write failed for switch bitmap: "+String(_switchBitmap)+", re-initializing board");
    }
    begin();
}

/**
 * loop()
 */
void NCD8Switch::loop()
{
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            notify();
        }
    }

//    //TODO: Poll switch periodically (.25 seconds?),
//    //      and publish MQTT message if it changes
//    long current = millis();
//    if(current > _lastPollTime + POLL_INTERVAL_MILLIS)
//    {
//        _lastPollTime = current;
//        bool newIsOn = isSwitchOn();
//        if(newIsOn != _isOn) {
//            _isOn = newIsOn;
//            publish("patriot/" + _name + "/get/position", _isOn ? "100" : "0" );
//        }
//    }
}

// Private Helper Methods
/**
 * isTimeToCheckSwitch()
 * @return bool if enough time has elapsed to sample switch again
 */
bool NCD8Switch::isTimeToCheckSwitch()
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
bool NCD8Switch::didSwitchChange()
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
void NCD8Switch::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}


