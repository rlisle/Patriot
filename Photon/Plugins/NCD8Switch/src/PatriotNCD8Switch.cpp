/******************************************************************
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

 ******************************************************************/

#include "PatriotNCD8Switch.h"

#define MILLIS_PER_SECOND 1000
#define POLL_INTERVAL_MILLIS 250

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param switchNum is the switch number on the NCD 8 GPIO board (1-8)
 * @param name String name used in MQTT messages
 */
NCD8Switch::NCD8Switch(int address, int switchNum, String name)
                : Device(name)
{
    _address = address;
    _lastPollTime = 0;
    _lastState    = 0;
    _switchBitmap = 0;
    _type         = 'S';
    
    if(_switchNum > 0 && switchNum <= 8) {
        _switchBitmap = 0x01 << (switchNum-1);
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
        Wire.beginTransmission(_address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xff);                   // Set all 8 to inputs
        status = Wire.endTransmission();    // Write 'em, Dano
        
        Wire.beginTransmission(_address);
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
        Wire.beginTransmission(_address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
    }
    
    Wire.requestFrom(_address, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return((data & _switchBitmap) == 0);    // Inverted
    }
    Log.error("Error reading switch");
    return false;
}

/**
 * loop()
 */
void NCD8Switch::loop()
{
    //TODO: Poll switch periodically (.25 seconds?),
    //      and publish MQTT message if it changes
    long current = millis();
    if(current > _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        _lastPollTime = current;
        bool newIsOn = isSwitchOn();
        if(newIsOn != _isOn) {
            _isOn = newIsOn;
            publish("patriot/" + _name, _isOn ? "100" : "0" );
        }
    }
};
