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
   https://cdn-shop.adafruit.com/datasheets/MCP23008.pdf
 
 Changelog:
 2021-06-20: Initial creation
 2022-12-10: Change MQTT message to patriot/<device>/get/position
 2023-08-23: Refactor MCP23008 to separate class
 */

#include "IoT.h"

#define POLL_INTERVAL_MILLIS 250

/**
 * Constructor
 * @param switchIndex is the switch number on the NCD 8 GPIO board (1-8)
 * @param name String name used in MQTT messages
 */
NCD8Switch::NCD8Switch(int switchIndex, String name, String room)
                : Device(name, room)
{
    _lastPollTime = 0;
    _type         = 'S';
    
    if(switchIndex > 0 && switchIndex <= 8) {
        _switchBitmap = 0x01 << (switchIndex-1);
    } else {
        _switchBitmap = 0x01;   // If 0 or invalid, set to first switch
    }
}

/**
 * Private Methods
 */

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD8Switch::isSwitchOn() {
    int data = MCP23008::read();
    return(data & _switchBitmap);    // Inverted by IPOL register
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

    if(oldState == false && newValue == 100) {
        _value = 100;
        return true;
    }

    if(oldState == true && newValue == 0) {
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


