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

#define MILLIS_PER_SECOND 1000

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
Relay::Relay(int8_t address, int8_t numRelays, int8_t relayNum, String name, int8_t duration)
    : Device(name)
{
    _relayNum   = relayNum;
    _percent    = 0;
    _duration   = duration;
    _stopMillis = 0;

    switch(numRelays)
    {
        case 8:
        default:
            _boardIndex = initialize8RelayBoard(address);
            break;
    }
}

int8_t Relay::initialize8RelayBoard(int8_t address) {

    _registerAddress = 0x0A;    // Does this change for different boards?

    int8_t index = boardIndex(address);
    if(index < 0)
    {
        index = initializeBoard(address);
    }
    return index;
}

int8_t Relay::initializeBoard(int8_t address) {

    // Only the first relay loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
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
 * Set percent
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = off, >0 = on
 */
void Relay::setPercent(int percent) {
    if(percent == 0) setOff();
    else setOn();
}

/**
 * Set On
 */
void Relay::setOn() {
    if(isOn()) return;

    _percent = 100;

    if(_duration != 0)
    {
        _stopMillis = millis() + (_duration * MILLIS_PER_SECOND);
    }

    byte bitmap = 1 << _relayNum;
    Relay::_currentStates[_boardIndex] |= bitmap;            // Set relay's bit

    Wire.beginTransmission(_addresses[_boardIndex]);
    Wire.write(_registerAddress);
    Wire.write(Relay::_currentStates[_boardIndex]);
    byte status = Wire.endTransmission();
    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning on relays");
    }
}

/**
 * Set relay off
 */
void Relay::setOff() {
    if(isOff()) return;

    _percent = 0;
    _stopMillis = 0;

    byte bitmap = 1 << _relayNum;
    bitmap = 0xff ^ bitmap;
    Relay::_currentStates[_boardIndex] &= bitmap;

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
 * Private Methods
 */

/**
 * loop()
 */
void Relay::loop()
{
    if(_stopMillis != 0)
    {
        if(millis() >= _stopMillis)
        {
            _stopMillis = 0;
            setOff();
        }
    }
};
