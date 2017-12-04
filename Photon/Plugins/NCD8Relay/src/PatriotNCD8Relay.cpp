/******************************************************************
 NCD 8 Relay board control

 Up to 8 relay boards can reside on a single I2C bus.

 Features:
 - On/Off control
 - Supports multiple boards
 - Automatic shut off if duration specified

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-12-03: Add retry.
 2017-10-03: Initial creation
 ******************************************************************/

#include "PatriotNCD8Relay.h"

#define MILLIS_PER_SECOND 1000

int8_t NCD8Relay::_numControllers = 0;  // Count of relay boards on I2C bus
int8_t NCD8Relay::_currentStates[8];    // All relays initially off
int8_t NCD8Relay::_addresses[8];        // Addresses of up to 8 boards

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param numRelays identifies which NCD Relay board by the number of relays on it
 * @param relayNum is the relay number on the NCD 8 Relay board (1-8)
 * @param name String name used to address the relay.
 * @param duration Optional seconds value to automatically turn off relay. 0 = no automatic turn off.
 */
NCD8Relay::NCD8Relay(int8_t address, int8_t numRelays, int8_t relayNum, String name, int8_t duration)
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

int8_t NCD8Relay::initialize8RelayBoard(int8_t address) {

    _registerAddress = 0x0A;    // Does this change for different boards?

    int8_t index = boardIndex(address);
    if(index < 0)
    {
        index = initializeBoard(address);
    }
    return index;
}

int8_t NCD8Relay::initializeBoard(int8_t address) {
    int retries;
    byte status;

    // Only the first relay loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0x00);                   // Set all 8 to outputs
        status = Wire.endTransmission();    // ??? Write 'em, Dano

        // Note: pull-ups should have no effect on outputs, so this looks wrong.
        Wire.write(0x06);                   // ??? Select pull-up resistor register
        Wire.write(0x00);                   // ??? pull-ups disabled on all 8 outputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);

    if(status != 0) {
        Serial.println("Set address failed");
        //TODO: handle error
    }

    return addAddressToArray(address);
}

int8_t NCD8Relay::boardIndex(int8_t address) {
    for(int8_t index=0; index<_numControllers; index++) {
        if(_addresses[index] == address) {
            return index;
        }
    }
    return -1;
}

int8_t NCD8Relay::addAddressToArray(int8_t address) {
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
void NCD8Relay::setPercent(int percent) {
    if(percent == 0) setOff();
    else setOn();
}

/**
 * Set On
 */
void NCD8Relay::setOn() {

    if(isOn()) return;

    _percent = 100;

    if(_duration != 0)
    {
        _stopMillis = millis() + (_duration * MILLIS_PER_SECOND);
    }

    byte bitmap = 1 << _relayNum;
    NCD8Relay::_currentStates[_boardIndex] |= bitmap;            // Set relay's bit

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_addresses[_boardIndex]);
        Wire.write(_registerAddress);
        Wire.write(NCD8Relay::_currentStates[_boardIndex]);
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning on relays");
    }
}

/**
 * Set relay off
 */
void NCD8Relay::setOff() {
    if(isOff()) return;

    _percent = 0;
    _stopMillis = 0;

    byte bitmap = 1 << _relayNum;
    bitmap = 0xff ^ bitmap;
    NCD8Relay::_currentStates[_boardIndex] &= bitmap;

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_addresses[_boardIndex]);
        Wire.write(_registerAddress);
        Wire.write(NCD8Relay::_currentStates[_boardIndex]);
        byte status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

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
void NCD8Relay::loop()
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
