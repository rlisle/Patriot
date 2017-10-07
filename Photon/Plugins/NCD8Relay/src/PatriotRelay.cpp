/******************************************************************
 Relay control

 Up to 8 relay controllers can reside on a single I2C bus.

 Features:
 - On/Off control

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-10-03: Initial creation
 ******************************************************************/

#include "PatriotRelay.h"

int8_t Relay::_numControllers = 0;    // Count of relay boards on I2C bus
int8_t Relay::_currentStates[8];      // All relays initially off
int8_t Relay::_addresses[8];          // Addresses of up to 8 boards

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param numRelays identifies which NCD Relay board by the number of relays on it
 * @param relayNum is the relay number on the NCD 8 Relay board (1-8)
 * @param name String name used to address the relay.
 */
Relay::Relay(int8_t address, int8_t numRelays, int8_t relayNum, String name, uint8_t duration=0)
{
    Serial.println("Debug: creating relay "+name);

    _relayNum   = relayNum;
    _name       = name;
    _percent    = 0;
    _duration   = duration;

    switch(numRelays)
    {
        case 8:
        default:
            _boardIndex = initialize8RelayBoard(address);
            break;
    }
}

int8_t Relay::initialize8RelayBoard(int8_t address) {
    Serial.println("Initializing board: " + String(address) + " relay #" + String(_relayNum));

    _registerAddress = 0x0A;    // Does this change for different boards?

    int8_t index = boardIndex(address);
    if(index < 0)
    {
        index = initializeBoard(address);
    }
    return index;
}

int8_t Relay::initializeBoard(int8_t address) {

    Serial.println("Adding new board "+String(address));

    // Only the first relay loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Serial.println("Initializing Wire (one time only!)");
        Wire.begin();
    }

    Wire.beginTransmission(address);
    Wire.write(0x00);                   // Select IO Direction register
    Wire.write(0x00);                   // Set all 8 to outputs
    Wire.endTransmission();             // ??? Write 'em, Dano

    // Note: pull-ups should have no effect on outputs, so this looks wrong.
    Wire.write(0x06);                   // ??? Select pull-up resistor register
    Wire.write(0x00);                   // ??? pull-ups disabled on all 8 outputs
    Wire.endTransmission();

    Serial.println("Turning off all relays");

    return addAddressToArray(address);
}

int8_t Relay::boardIndex(int8_t address) {
    for(int8_t index=0; index<_numControllers; index++) {
        if(_addresses[index] == address) {
            return index;
        }
    }
    return -1;
}

int8_t Relay::addAddressToArray(int8_t address) {
    _currentStates[_numControllers] = 0x00;
    _addresses[_numControllers] = address;

    Wire.write(_registerAddress);
    Wire.write(0x00);                      // Turn off all relays
    byte status = Wire.endTransmission();
    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning off relays");
    }
    return _numControllers++;
}

/**
 * name
 * @return String name of relay
 */
String Relay::name() {
    return _name;
}


/**
 * Set percent
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = off, >0 = on
 */
void Relay::setPercent(int percent) {
    Serial.println("DEBUG: setPercent: "+String(percent));
    if(percent == 0) setOff();
    else setOn();
}

/**
 * Set On
 */
void Relay::setOn() {
    if(isOn()) return;

    _percent = 100;
    Serial.println("DEBUG: setOn "+String(_relayNum));

    byte bitmap = 1 << _relayNum;
    Relay::_currentStates[_boardIndex] |= bitmap;            // Set relay's bit

    Wire.beginTransmission(_addresses[_boardIndex]);
    Wire.write(_registerAddress);
    Wire.write(Relay::_currentStates[_boardIndex]);
    byte status = Wire.endTransmission();
    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning off relays");
    }
}

/**
 * Set relay off
 */
void Relay::setOff() {
    if(isOff()) return;

    _percent = 0;
    Serial.println("DEBUG: setOff "+String(_relayNum));

    byte bitmap = 1 << _relayNum;
    bitmap = !bitmap;
    Serial.println("DEBUG: bitmap = "+String(bitmap));
    Relay::_currentStates[_boardIndex] &= bitmap;
    Serial.println("DEBUG: new state = "+String(_currentStates[_boardIndex]));

    Wire.beginTransmission(_addresses[_boardIndex]);
    Wire.write(_registerAddress);
    Wire.write(Relay::_currentStates[_boardIndex]);
    byte status = Wire.endTransmission();
    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning off relays");
    }
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool Relay::isOn() {
    return _percent != 0;
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool Relay::isOff() {
    return _percent == 0;
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void Relay::loop()
{
    // Nothing to do
};
