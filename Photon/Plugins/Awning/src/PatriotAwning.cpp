/**
 Patriot Awning

 This plugin supports connection to a Carefree BT12 automated awning.
 The awning opens when the <color> wire is connected to ground <color> wire for .5 second,
 and closes when <color> wire is connect to ground <color> wire for .5 seconds.
 Motion is stopped when the same wire is briefly connected again.

 This plugin is designed to coexist with up to 2 NCD4Relays
 and up to 4 NCD4Switch devices using the GPIOs on same board.
 
 Features:
 - 2 relay automated awning control
 - On (open), Off (closed), or timed partially open (1-99)
 - Supports multiple boards
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2023-05-28: Initial creation based on PatriotCurtain
 */

#include "PatriotAwning.h"
#include "IoT.h"

#define MILLIS_PER_SECOND 1000
#define FULL_TIME_MILLIS 6000   // Duration of full open to close or vis-vis
#define MILLIS_PER_PERCENT 60   // FULL_TIME_MILLIS / 100
//#define MILLIS_PER_UPDATE 250   // Send updates every 1/4 second
#define MILLIS_PER_UPDATE 1000  // Send updates every second
#define PULSE_MILLIS 100

// Mode (relay bit)
#define OPEN_AWNING 2
#define CLOSE_AWNING 1

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7)
 * @param relayIndex is the relay number of the 1st of 2 relays (0-2)
 * @param name String name used to address the relay.
 */
Awning::Awning(int8_t boardAddress, int8_t relayIndex, String name, String room)
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

void Awning::begin() {

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
void Awning::setValue(int percent) {

    //FOR DEBUGGING, SETTING ONLY OPEN OR CLOSED
    
    if(percent == _value) {
        Log.warn("Awning setValue is the same as previous value, ignoring");
        //TODO: do we want to issue open/close again just in case?
        return;
    }

    _startPosition = _value;
    _startMillis = millis();
    
    _value = percent;           // Should this report current instead?
    _holding = false;
    _updateMillis = millis() + MILLIS_PER_UPDATE;
    Log.info("_updateMillis = %ld",_updateMillis);

    // Send HomeKit acknowledgement
    IoT::publishMQTT("/ack/" + _name + "/set",String(percent));
    
    // Send position updates
    IoT::publishMQTT(_name + "/get",String(percent));
    IoT::publishMQTT(_name + "/position",String(_startPosition));

    if(_value > _startPosition) {
        _mode = OPEN_AWNING;
        IoT::publishMQTT(_name + "/state", "increasing");

    } else {
        _mode = CLOSE_AWNING;
        IoT::publishMQTT(_name + "/state", "decreasing");
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

void Awning::setHold(bool holding) {
    if(holding == true) {
        if(_holding == true) {  // Already holding?
            return;
        }
        //TODO: stop movement, but remember target in case hold false
        
    } else {        // resume
        if(_holding == false) { // Not currently holding?
            return;
        }
        //TODO: resume movement - same as setValue using old target
        
    }
    _holding = holding;
    Log.warn("Awning setHold not implemented");
}


/**
 * Start 1=close, 2=open
 */
void Awning::pulse(bool high) {
    
    Log.info("Awning pulse %s",high ? "high" : "low");

    int currentState = readCurrentState();
    
    byte bitmap = 0x01 << (_relayIndex + _mode - 1);
    if(high) {
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
        Log.error("Error pulsing relay %d %s", bitmap, high ? "high" : "low");
    }
}

/**
 * loop()
 */
void Awning::loop()
{
    if(isAwningRunning()) {
        
        if(isTimeToChangePulse()) {
            switch(_stage) {
                case 1:
                    Log.info("Awning end-of-start pulse");
                    pulse(false);
                    
                    _stopMillis = millis() + ((FULL_TIME_MILLIS *  abs(_startPosition - _value)) / 100) - PULSE_MILLIS;
                    
                    _stage = 2;
                    break;
                case 2:
                    Log.info("Awning start-of-end pulse");
                    pulse(true);
                    _stopMillis = millis() + PULSE_MILLIS;
                    _stage = 3;
                    break;
                case 3:
                    Log.info("Awning end-of-end pulse");
                    pulse(false);
                    _stopMillis = millis() + FULL_TIME_MILLIS - PULSE_MILLIS;
                    _stage = 4;
                    break;
                case 4:
                    _stage = 0;
                    IoT::publishMQTT(_name + "/state", "stopped");
                    break;
                default:
                    Log.error("Invalid _stage %d",_stage);
            }
            
        }
        
        // Calculate periodic HomeKit updates to getCurrentPosition
        if(millis() >= _updateMillis) {
            _updateMillis += MILLIS_PER_UPDATE;
            
            int percentDelta = (int)((millis() - _startMillis) / MILLIS_PER_PERCENT);
            if(_mode == CLOSE_AWNING) percentDelta = -percentDelta;
//            Log.info("DBG: _startPosition = %d",_startPosition);
//            Log.info("DBG: awning percent delta = %d",percentDelta);
//            Log.info("DBG: _startMillis = %ld",_startMillis);
//            Log.info("DBG: millis = %ld",millis());
//            Log.info("DBG: millis delta = %ld",millis() - _startMillis);
//            _value = _startPosition + percentDelta;
//            IoT::publishMQTT(_name + "/position", String(_value));
            IoT::publishMQTT(_name + "/position", String(_startPosition + percentDelta));
        }
    }
};

bool Awning::isAwningRunning() {
    return(_stage != 0);
}

bool Awning::isTimeToChangePulse() {
    return(millis() >= _stopMillis);
}

void Awning::reset() {
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
        Log.error("Awning reset write failed for relayIndex: "+String(_relayIndex)+", re-initializing board");
    }
    begin();
}

/**
 * readCurrentState
 * Return state of all 4 relays
 */
int Awning::readCurrentState() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Awning: Error selecting GPIO register");
    }
    
    Wire.requestFrom(_boardAddress, 1);      // Read 1 byte
    
    if (Wire.available() != 1)
    {
        Log.error("Awning: Error reading current state");
        return 0;
    }
    
    int data = Wire.read();
    return(data);
}

