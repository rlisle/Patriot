/**
 Patriot Curtain

 This plugin supports connection to an automated curtain.
 The curtain opens when pin 6 is briefly connected to pin 1 ground,
 and closes when pin 5 is briefly connect to pin 1 ground.
 Motion is stopped when the same pin is briefly connected again.

 This plugin is designed to coexist with up to 2 NCD4Relay
 and other NCD4Switch devices using the same board.
 
 Features:
 - 2 relay automated curtain control
 - On (open), Off (closed), or timed partially open (1-99)
 - Supports multiple boards
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-01-16: Initial creation based on NCD8Relay
 */

#include "PatriotCurtain.h"

#define MILLIS_PER_SECOND 1000
// Adjust this to match length of time to open/close curtains
#define FULL_TIME_MILLIS 10000
#define PULSE_MILLIS 200

// Mode (relay bit)
#define OPEN_CURTAIN 2
#define CLOSE_CURTAIN 1

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7)
 * @param relayIndex is the relay number of the 1st of 2 relays (0-2)
 * @param name String name used to address the relay.
 */
Curtain::Curtain(int8_t boardAddress, int8_t relayIndex, String name)
    : Device(name, DeviceType::Curtain)
{
    _boardAddress = boardAddress;   // 0x20 (no jumpers)
    _relayIndex  = relayIndex;
    _currentState = 0;
    _stopMillis = 0;
    _mode = 0;
    _stage = 0;
}

void Curtain::begin() {

    byte status;

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    int retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xf0);                   // 0-3 out, 4-7 in
        status = Wire.endTransmission();    // Write 'em, Dano

        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0);                   // pull-ups enabled on inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);

    if(status != 0) {
        Log.error("Set address failed");
        //TODO: handle error
    }
}

/**
 * Set value
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = closed, >0 = open
 */
void Curtain::setValue(int percent) {

    _previous = value;
    _value = percent;
    
    if(_value > _previous) {
        _mode = OPEN_CURTAIN
    } else {
        _mode = CLOSE_CURTAIN;
    }

    _stage = 1;
    _stopMillis = Millis() + PULSE_MILLIS;
    pulse(true);
}

/**
 * Start 1=close, 2=open
 */
void Curtain::pulse(bool start) {
    
    byte bitmap = relay << _mode;
    if(start) {
        _currentState |= bitmap;    // Set relay's bit
    } else {
        bitmap = 0xff ^ bitmap;
        _currentState &= bitmap;
    }

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);
        Wire.write(_currentState);
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Log.error("Error turning on relays");
    }
}

/**
 * loop()
 */
void Curtain::loop()
{
    if(isCurtainRunning() && isTimeToChangePulse()) {
    switch(_stage) {
        case 1:
            pulse(false);
            _stopMillis = millis() + (FULL_TIME_MILLIS * 100 / abs(delta)) - PULSE_MILLIS;
            _stage = 2;
            break;
        case 2:
            pulse(true);
            _stopMillis = millis() + PULSE_MILLIS;
            _stage = 3;
            break;
        case 3:
            pulse(false);
            _stage = 0;
            break;
        default:
            // Nothing to do
        }
    }
};

bool isCurtainRunning() {
    return(_stage != 0);
}

bool isTimeToChangePulse() {
    return(millis() >= _stopMillis);
}
