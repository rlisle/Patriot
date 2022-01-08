/**
 NCD 4 Relay board control

 Features:
 - On/Off control of up to 4 relays on I2C4R4IO board

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "PatriotNCD4Relay.h"

#define MILLIS_PER_SECOND 1000
#define REGISTER_ADDRESS 0x0A

int8_t NCD4Relay::_currentState;   // Bitmap of relay states
int8_t NCD4Relay::_address;        // Addresses of board

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param relayNum is the relay number on the NCD 4 Relay board (1-4)
 * @param name String name used to address the relay.
 * @param room String name of location room
 */
NCD4Relay::NCD4Relay(int8_t address, int8_t relayNum, String name, String room)
    : Device(name, room)
{
    _address    = address;   // 0x20 (no jumpers)
    _relayNum   = relayNum;
    _value      = 0;
    _type       = 'R';
}

void NCD4Relay::begin() {

    initializeBoard();
    
    // Initialize relay to off
    _value = 55;  // Just to allow setOff to not ignore
    setOff();
}

void NCD4Relay::initializeBoard() {
    
    int retries;
    byte status;

    // Only the first relay loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0x00);                   // Set all 8 to outputs
        status = Wire.endTransmission();    // ??? Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
    }

    retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(0x06);                   // ??? Select pull-up resistor register
        Wire.write(0x00);                   // ??? pull-ups disabled on all 8 outputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
        //TODO: handle error
    }
}

/**
 * Set percent
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = off, >0 = on
 */
void NCD4Relay::setPercent(int percent) {
    if(percent == 0) setOff();
    else setOn();
}

/**
 * Set On
 */
void NCD4Relay::setOn() {
    
    _value = 100;

    byte bitmap = 1 << _relayNum;
    NCD4Relay::_currentState |= bitmap;            // Set relay's bit

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(REGISTER_ADDRESS);
        Wire.write(NCD4Relay::_currentState);
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
void NCD4Relay::setOff() {
    
    _value = 0;

    byte bitmap = 1 << _relayNum;
    bitmap = 0xff ^ bitmap;
    NCD4Relay::_currentState &= bitmap;

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_address);
        Wire.write(REGISTER_ADDRESS);
        Wire.write(NCD4Relay::_currentState);
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
void NCD4Relay::loop()
{
    // Nothing to do
};
