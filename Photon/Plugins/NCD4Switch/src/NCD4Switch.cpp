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
 */

#include "PatriotNCD4Switch.h"

#define POLL_INTERVAL_MILLIS 100

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7) 0x20-0x27
 * @param relayIndex is the relay number of the 1st of 2 relays (0-2)
 * @param name String name used to address the relay.
 */
NCD4Switch::NCD4Switch(int8_t boardAddress, int8_t switchIndex, String name)
    : Device(name, DeviceType::NCD4Switch)
{
    _boardAddress = boardAddress;   // 0x20 (no jumpers)
    _lastPollTime = 0;
    
    if(_switchIndex > 0 && switchIndex <= 3) {
        _switchBitmap = 0x10 << switchIndex;
    } else {
        _switchBitmap = 0x10;   // If invalid, set to first switch
    }
}

void NCD4Switch::begin() {

    byte status;

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    int retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xf0);                   // 0-3 out, 4-7 in
        status = Wire.endTransmission();    // Write 'em, Dano

        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0);                   // pull-ups enabled on inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);

    if(status != 0) {
        Log.error("Set address failed");
        //TODO: handle error
    }
}

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD4witch::isSwitchOn() {
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
    Wire.beginTransmission(_address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD4Switch reset write failed for light "+String(_lightNum)+", re-initializing board");
    }
    initializeBoard();
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
    if (newValue == _value)
    {
        return false;
    }
    _value = newValue;
    return true;
}


/**
 * notify()
 * Publish switch state
 */
void NCD4Switch::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}

