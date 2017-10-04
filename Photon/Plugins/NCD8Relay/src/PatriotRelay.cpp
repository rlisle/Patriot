/******************************************************************
 Relay control

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

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param numRelays identifies which NCD Relay board by the number of relays on it
 * @param relayNum is the relay number on the NCD 8 Relay board (1-8)
 * @param name String name used to address the relay.
 */
Relay::Relay(byte address, byte numRelays, byte relayNum, String name)
{
    Particle.publish("DEBUG", "Create relay "+name+" for relay "+String(relayNum), 60, PRIVATE);

    _relayNum   = relayNum;
    _name       = name;
    _isOn       = false;

    switch(numRelays)
    {
        case 8:
        default:
            initialize8RelayBoard(address);
            break;
    }
}

void Relay::initialize8RelayBoard(byte address)
{
    _address = 0x20 + address;
    _registerAddress = 0x0A;

    // Only the first relay loaded needs to initialize the I2C link
    if(Wire.isEnabled()) return;

    // Note: This is the sequence from the NCD 8 Relay library.
    //       Lines with ??? appear to be wrong.
    Wire.begin();

    Wire.beginTransmission(_address);
    Wire.write(0x00);                   // Select IO Direction register
    Wire.write(0x00);                   // Set all 8 to outputs
    Wire.endTransmission();             // ??? Write 'em, Dano

    // Note: pull-ups should have no effect on outputs, so this looks wrong.
    Wire.write(0x06);                   // ??? Select pull-up resistor register
    Wire.write(0x00);                   // ??? pull-ups disabled on all 8 outputs
    byte status = Wire.endTransmission();
    //TODO: handle any errors, retry, etc.

    _currentState = 0;                  // All 8 relays off initially
}

/**
 * name
 * @return String name of relay
 */
String Relay::name() {
    return _name;
}

/**
 * Set On
 */
void Relay::setOn() {
    if(isAlreadyOn()) return;

    byte bitmap = 1 << _relayNum;
    _currentState |= bitmap;            // Set relay's bit

    Wire.beginTransmission(_address);
    Wire.write(_registerAddress);
    Wire.write(_currentState);
    byte status = Wire.endTransmission();
    //TODO: handle errors/retries
}

/**
 * Set relay off
 */
void Relay::setOff() {
    if(isAlreadyOff()) return;

    byte bitmap = 1 << _relayNum;
    bitmap = !bitmap;
    _currentState &= bitmap;

    Wire.beginTransmission(_address);
    Wire.write(_registerAddress);
    Wire.write(_currentState);
    byte status = Wire.endTransmission();
    //TODO: handle errors/retries
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool Relay::isOn() {
    byte mask = 1 << _relayNum;
    return _currentState & mask;
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool Relay::isOff() {
    return !isOn();
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void Light::loop()
{
    // Nothing to do
};
