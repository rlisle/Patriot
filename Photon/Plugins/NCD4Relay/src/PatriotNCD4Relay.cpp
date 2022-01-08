/**
 NCD 4 Relay board control

 Up to 4 relay boards can reside on a single I2C bus.
 
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

 */

#include "PatriotNCD4Relay.h"

#define MILLIS_PER_SECOND 1000

int8_t NCD4Relay::_numControllers = 0;  // Count of relay boards on I2C bus
int8_t NCD4Relay::_currentStates[4];    // All relays initially off
int8_t NCD4Relay::_addresses[4];        // Addresses of up to 8 boards

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param relayNum is the relay number on the NCD 4 Relay board (1-4)
 * @param name String name used to address the relay.
 * @param duration Optional seconds value to automatically turn off relay. 0 = no automatic turn off.
 */
NCD4Relay::NCD8Relay(int8_t address, int8_t relayNum, String name, String room)
    : Device(name, room)
{
    _boardAddress = address;   // 0x20 (no jumpers)
    _relayNum   = relayNum;
    _percent    = 0;
    _stopMillis = 0;
    _type       = 'R';
}

void NCD4Relay::begin() {

    _boardIndex = initialize4RelayBoard(address);
    
    // Initialize relay to off
    _percent = 55;  // Just to allow setOff to not ignore
    setOff();
}

int8_t NCD4Relay::initialize4RelayBoard(int8_t address) {

    _registerAddress = 0x0A;

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
        Log.error("Set address failed");
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
    //_currentStates[_numControllers] = 0x00; Leave as set in SRAM
    _addresses[_numControllers] = address;

    // // This looks wrong. Probably delete remainder of code here.
    // Wire.write(_registerAddress);
    // Wire.write(0x00);                      // Turn off all relays
    // byte status = Wire.endTransmission();
    // if(status != 0) {
    //     //TODO: handle any errors, retry, etc.
    //     Log.error("Error turning off relays");
    // }
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

    if(_percent > 0) return;

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
        Log.error("Error turning on relays");
    }
}

/**
 * Set relay off
 */
void NCD8Relay::setOff() {
    if(_percent == 0) return;

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
        Log.error("Error turning off relays");
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
