/**
 NCD PCA9685 16-channel 12-Bit PWM board control

 Up to 8 I2C boards can reside on a single I2C bus. ???
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Light library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:
    * PCA9685: https://media.ncd.io/sites/2/20170721134542/PCA9685.pdf
 */

#include "PatriotNCD16Dimmer.h"
//#include "math.h"

#define MILLIS_PER_SECOND 1000

/**
 * Constructor
 * @param address is the board address set by 6 jumpers: 0x01 if low switch set, 0x40 if high
 * @param lightNum is the channel number on the NCD 16 Dimmer board (0 based: 0 - 15) Note older plugins appear 1 based (bug?)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD16Dimmer::NCD16Dimmer(int8_t address, int8_t lightNum, String name, String room, int8_t duration)
                     : Device(name, room)
{
    _address = address;
    _lightNum   = lightNum;
    _dimmingDuration = duration * 1000; // Convert to msecs
    _value = 0;
    _currentLevel = 0;
    _targetLevel = 0;
    _incrementPerMillisecond = 0;
    _lastUpdateTime = 0;
    _type = 'L';
}

void NCD16Dimmer::begin() {
    initializeBoard();
}

int8_t NCD16Dimmer::initializeBoard() {
    int status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    Wire.beginTransmission(_address);   // Seems unnecessary
    status = Wire.endTransmission();

    //TODO: retry several times?
    if(status == 0) {
// NCD8 code commented out
//        Wire.beginTransmission(_address);
//        Wire.write(0);          // Control register - No AI, point to reg0 Mode1
//        Wire.write(0);          // Mode1 reg. Osc on, disable AI, subaddrs, allcall
//        Wire.endTransmission();
//
//        Wire.beginTransmission(_address);
//        Wire.write(1);          // Mode2 register
//        Wire.write(0x04);       // Dimming, Not inverted, totem-pole
//        Wire.endTransmission();
//
//        Wire.beginTransmission(_address);
//	    Wire.write(0x8c);       // AI + LEDOUT0
//	    Wire.write(0xaa);       // LEDOUT0 LEDs 0-3 dimming
//	    Wire.write(0xaa);       // LEDOUT1 LEDS 4-7 dimming
//        Wire.endTransmission();
        // PCA9685 repo code
        Wire.beginTransmission(_address);
        Wire.write(254);    // PRE_SCALE for PWM output freq
        Wire.write(5);      // 5 = ?
        Wire.endTransmission();
        
        Wire.beginTransmission(_address);
        Wire.write(0);      // MODE1
        Wire.write(161);    // 128=Restart Enabled, internal clock, 32=AutoIncrement, 1=AllCall
        // MODE2 left default: NotInverted, Output on Stop command, OpenDrain, OUTNE
        Wire.endTransmission();

        outputPWM();            // Force light off
        
        Log.info("InitializeBoard " + _name + " sucess");
        
    } else {
        Log.error("InitializeBoard " + _name + " FAILED!");
    }

    return status;
}

void NCD16Dimmer::reset() {
    Log.error("Resetting 9685board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9685 SWRST - this doesn't look correct, so commented out for now
//    Wire.beginTransmission(_address);
//    Wire.write(0x06);       // Isn't this LED0???
//    Wire.write(0xa5);
//    Wire.write(0x5a);
//    byte status = Wire.endTransmission();
//    if(status != 0){
//        Log.error("NCD16Dimmer reset write failed for light "+String(_lightNum)+", reseting Wire");
//    }
    initializeBoard();
}

/**
 * Set value
 * @param value Int 0 to 100.
 */
void NCD16Dimmer::setValue(int value) {
    if( value == _value ) {
        Log.info("Dimmer " + _name + " setValue " + String(value) + " same so outputPWM without dimming");
        _currentLevel = scalePWM(_value);   // Turn 0-100 into 0x0 to 0x7fffffff
        outputPWM();
        return;
    }
    
    _currentLevel = scalePWM(_value);   // previous value
    _value = value;                     // percent 0-100
    _targetLevel = scalePWM(value);     // new 32 bit value
    Log.info("Dimmer " + _name + " setValue " + String(value) + " scaled to " + String(_targetLevel));
    if(_dimmingDuration == 0) {
        _currentLevel = _targetLevel;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use float _currentValue to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void NCD16Dimmer::startSmoothDimming() {
    if((int)_currentLevel != _targetLevel){
        _lastUpdateTime = millis();
        int delta = _targetLevel - _currentLevel;   // signed 32 bit
        _incrementPerMillisecond = delta / _dimmingDuration;
        Log.info("Light "+_name+" setting increment to "+String(_incrementPerMillisecond));
    }
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void NCD16Dimmer::loop()
{
    // Is fading transition underway?
    if(_currentLevel == _targetLevel) {
        return;
    }
    
    int loopTime = millis();
    int millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentLevel += _incrementPerMillisecond * millisSinceLastUpdate;
    if(_incrementPerMillisecond > 0) {
        if(_currentLevel > _targetLevel || _currentLevel < 0) { // Check for overflow also
            _currentLevel = _targetLevel;
        }
    } else {
        if(_currentLevel < _targetLevel) {
            _currentLevel = _targetLevel;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM _currentLevel (0-4095)
 */
void NCD16Dimmer::outputPWM() {
    int reg = 6 + (_lightNum * 4); // 0 based, not 1 based like older plugins

    int current4k = _currentLevel >> 19; // Convert 0-0x7fffffff to 0-0xfff
    int lsb = current4k & 0xff;
    int msb = current4k >> 8;
    
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(_address);
        Wire.write(reg);
        Wire.write(0);      // Delay lsb
        Wire.write(0);      // Delay msb
        Wire.write(lsb);    // Off lsb
        Wire.write(msb);    // Off msb
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0) {
        Log.error("NCD16Dimmer outputPWM write failed for light "+String(_lightNum)+", level = "+String(current4k));
        reset();
    }
}

/**
 * Convert 0-100 to 0-0x7fffffff (exponential?) scale
 * 0 = 0, 100 = 0x7fffffff
 */
float NCD16Dimmer::scalePWM(int value) {
    if (value <= 0) return 0;
    if (value >= 100) return 0x7fffffff;
    
    //TODO: No curve applied
    return value * (0x7fffffff / 100)
}
