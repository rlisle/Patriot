/**
 NCD GPIO Switch control

 Uses the MCP23017
 
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

*/

#include "PatriotNCD16Switch.h"

#define MILLIS_PER_SECOND 1000
#define POLL_INTERVAL_MILLIS 250

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param switchNum is the switch number on the NCD 8 GPIO board (1-8)
 * @param name String name used in MQTT messages
 */
NCD16Switch::NCD16Switch(int address, int switchNum, String name)
                : Device(name)
{
    _address = address;
    _lastPollTime = 0;
    _lastState    = 0;
    _switchBitmap = 0;
    
    if(switchNum > 0 && switchNum <= 16) {
        _switchBitmap = 0x01 << (switchNum-1);
    }
}

void NCD16Switch::begin() {
    if(switchBitmap == 0) {
        Log.error("Invalid switchNum");
        return;
    }
    initializeBoard();
}

/**
 * Private Methods
 */

int NCD16Switch::initializeBoard() {
    byte status;
    
    // Only the first switch loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    // Assumes default IOCON set (interleaved, sequential)
    Wire.beginTransmission(_address);
    Wire.write(0x00);                   // Select IO Direction register
    Wire.write(0xff);                   // Set first 8 to inputs
    Wire.write(0xff);                   // Set second 8 to inputs
    status = Wire.endTransmission();    // Write 'em, Dano
    
    Wire.beginTransmission(_address);
    Wire.write(0x0C);                   // Select pull-up resistor register
    Wire.write(0xff);                   // pull-ups enabled on first 8 inputs
    Wire.write(0xff);                   // pull-ups enabled on second 8 inputs
    status = Wire.endTransmission();
    
    if(status != 0) {
        Log.error("Initialize board failed");
    }
    
    return status;
}

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD16Switch::isSwitchOn() {
    int status;
    
    // We'll want to read 2 sequential bytes.
    Wire.beginTransmission(_address);
    Wire.write(0x12);       // GPIO Register
    status = Wire.endTransmission();
    if(status != 0) {
        Log.error("Error selecting GPIO register");
        return false;
    }
    
    // Changing reading 1 byte to 2. If sequential set, should work?
    Wire.requestFrom(_address, 2);      //
    
    if (Wire.available() >= 2)
    {
        int data1 = Wire.read();
        int data2 = Wire.read();
        int data = (data1 << 8) + data2;
        bool result = (data & _switchBitmap) == 0;  // Inverted
        Log.trace("Switch data read = "+String(data,BIN)+" AND "+String(_switchBitmap,BIN)+" = "+String(result));
        return(result);
    }
    Log.error("Error reading switch");
    return false;
}

/**
 * loop()
 */
void NCD16Switch::loop()
{
    // Poll switch periodically (.25 seconds?),
    // and publish MQTT message if it changes
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
