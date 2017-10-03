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
 * @param address is the NCD 8 Relay board address (0-7)
 * @param relayNum is the relay number on the NCD 8 Relay board (1-8)
 * @param name String name used to address the relay.
 */
Relay::Relay(int address, int relayNum, String name)
{
    Particle.publish("DEBUG", "Create relay "+name+" for relay "+String(relayNum), 60, PRIVATE);

    _address    = 0x20 + address;
    _relayNum   = relayNum;
    _name       = name;

    // Only the first relay needs to initialize the I2C link
    if(Wire.isEnabled()) return;

    Wire.begin();
    Wire.beginTransmission(address);
    Wire.write(0x00);   // Set all to outputs
    Wire.write(0x00);
    Wire.endTransmission(); // Write 'em, dano
    Wire.write(0x06);
    Wire.write(0x00);
    byte status = Wire.endTransmission();
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
    //TODO: turn relay on
}

/**
 * Set relay off
 */
void Relay::setOff() {
    if(isAlreadyOff()) return;
    setPercent(0);
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool Relay::isOn() {
    return !isOff();
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool Relay::isOff() {
    return _targetPercent == 0;
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
