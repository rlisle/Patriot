/**
 Patriot Curtain

 This plugin supports connection to an automated curtain.
 The curtain opens when pin 6 is briefly connected to pin 1 ground,
 and closes when pin 5 is briefly connect to pin 1 ground.
 Motion is stopped when the same pin is briefly connected again.

 This plugin is designed to coexist with up to 2 NCD4Relays
 and up to 4 NCD4Switch devices using the GPIOs on same board.
 
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
 2022-04-17: Fix issue turning off other relays on same board.
 */

#include "PatriotCurtain.h"
#include "IoT.h"

#define MILLIS_PER_SECOND 1000
// Adjust this to match length of time to open/close curtains
#define FULL_TIME_MILLIS 6000
#define PULSE_MILLIS 100

// Mode (relay bit)
#define OPEN_CURTAIN 2
#define CLOSE_CURTAIN 1

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7)
 * @param relayIndex is the relay number of the 1st of 2 relays (0-2)
 * @param name String name used to address the relay.
 */
Curtain::Curtain(int8_t boardAddress, int8_t relayIndex, String name, String room)
    : Device(name, room)
{
    _boardAddress = boardAddress;   // 0x20 (no jumpers)
    _relayIndex  = relayIndex;      // 0 (first 2 relays)
    _stopMillis = 0;
    _mode = 0;
    _stage = 0;
    _type  = 'C';
    _holding = false;
}

void Curtain::begin() {

    byte status;
    int  retries;

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                 // Select IO Direction register
        Wire.write(0xf0);                 // 0-3 out, 4-7 in
        status = Wire.endTransmission();  // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
    }

    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x06);        // Select pull-up resistor register
        Wire.write(0xf0);        // pull-ups enabled on inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
    }
}

/**
 * Set value
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = closed, >0 = open
 */
void Curtain::setValue(int percent) {

    if(_value == _previous) {
        Log.warn("Curtain setValue the same, ignoring");
        return;
    }
    
    _previous = _value;
    _value = percent;           // Should this report current instead?
    _holding = false;
    _startMillis = millis();

    // Send HomeKit acknowledgement
    IoT::mqttPublish(kPublishName + "/getTargetPosition/" + _name, String(percent));
    
    if(_value > _previous) {
        _mode = OPEN_CURTAIN;
        IoT::mqttPublish(kPublishName + "/getPositionState/" + _name, "increasing");

    } else {
        _mode = CLOSE_CURTAIN;
        IoT::mqttPublish(kPublishName + "/getPositionState/" + _name, "decreasing");
    }

    // We only need a single pulse if opening or closing all the way
    if(_value == 0 || _value == 100) {
        _stage = 3;
    } else {
        _stage = 1;
    }
    _stopMillis = millis() + PULSE_MILLIS;
    pulse(true);
}

void Curtain::setHold(bool holding) {
    _holding = holding;
    //TODO: stop curtain if moving
    Log.warn("Curtain setHold not implemented");
}


/**
 * Start 1=close, 2=open
 */
void Curtain::pulse(bool start) {
    
    Log.info("Curtain pulse %d",start);

    int currentState = readCurrentState();
    
    byte bitmap = 0x01 << (_relayIndex + _mode - 1);
    if(start) {
        currentState |= bitmap;    // Set relay's bit
    } else {
        bitmap = 0xff ^ bitmap;
        currentState &= bitmap;
    }

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);
        Wire.write(currentState);
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        Log.error("Error pulsing relay %d %d", bitmap, start);
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
                Log.info("Curtain end-of-start pulse");
                pulse(false);
                
                _stopMillis = millis() + ((FULL_TIME_MILLIS *  abs(_previous - _value)) / 100) - PULSE_MILLIS;
                
                _stage = 2;
                break;
            case 2:
                Log.info("Curtain start-of-end pulse");
                pulse(true);
                _stopMillis = millis() + PULSE_MILLIS;
                _stage = 3;
                break;
            case 3:
                Log.info("Curtain end-of-end pulse");
                pulse(false);
                _stopMillis = millis() + FULL_TIME_MILLIS - PULSE_MILLIS;
                _stage = 4;
                break;
            case 4:
                _stage = 0;
                IoT::mqttPublish(kPublishName + "/getPositionState/" + _name, "stopped");
                break;
            default:
                Log.error("Invalid _stage %d",_stage);
        }
    }
};

bool Curtain::isCurtainRunning() {
    return(_stage != 0);
}

bool Curtain::isTimeToChangePulse() {
    return(millis() >= _stopMillis);
}

void Curtain::reset() {
    Log.error("Resetting board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(_boardAddress);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("Curtain reset write failed for relayIndex: "+String(_relayIndex)+", re-initializing board");
    }
    begin();
}

/**
 * readCurrentState
 * Return state of all 4 relays
 */
int Curtain::readCurrentState() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Curtain: Error selecting GPIO register");
    }
    
    Wire.requestFrom(_boardAddress, 1);      // Read 1 byte
    
    if (Wire.available() != 1)
    {
        Log.error("Curtain: Error reading current state");
        return 0;
    }
    
    int data = Wire.read();
    return(data);
}

