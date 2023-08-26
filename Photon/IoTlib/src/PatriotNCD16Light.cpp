/**
 NCD PCA9685 16-channel 12-Bit PWM board control

 Up to 8 I2C boards can reside on a single I2C bus. ???
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified
 
 - The "Common Bus" appears to be isolated from everything else, so can be used as
  as a + power strip for the LEDs. It can be sourced separately if desired.
 
 Photon2 has native floating point, so no need to use integer math instead of FP.
 For now will include PCA9685 within this file.
 Once another board needs to use it, then will split it out into its own PCA9685 class.
 
 Deprecated:
 Native 32 bit signed int math will be used since Photon does not have native FP support.
 Since D/A is 12 bit, data will be converted from 32b signed to 12 bit unsigned (<< 20?)
 So fixed point math means upper 12 bits are unsigned value (0-4095) and lower 19 bits are fractional (0 - 524,287)
 leaving top bit for sign which is discarded during signed to unsigned conversion.
 Smallest value (1) = 524,288 (0x0008 0000) which when >> 19 = 001

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:
    * PCA9685: https://media.ncd.io/sites/2/20170721134542/PCA9685.pdf
 */

#include "IoT.h"

#define MILLIS_PER_SECOND 1000

/**
 * Constructor
 * @param address is the board address set by 6 jumpers: 0x01 if low switch set, 0x40 if high
 * @param lightNum is the channel number on the NCD 16 Dimmer board (0 based: 0 - 15) Note older plugins appear 1 based (bug?)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD16Light::NCD16Light(int8_t address, int8_t lightNum, String name, String room, int8_t duration)
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

void NCD16Light::begin() {
    initializeBoard();
}

int8_t NCD16Light::initializeBoard() {
    int status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    Wire.beginTransmission(_address);   // Seems unnecessary
    status = Wire.endTransmission();

    //TODO: retry several times?
    if(status == 0) {
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

void NCD16Light::reset() {
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
//        Log.error("NCD16Light reset write failed for light "+String(_lightNum)+", reseting Wire");
//    }
    initializeBoard();
}

/**
 * Set value
 * @param value Int 0 to 100.
 */
void NCD16Light::setValue(int value) {
    if( value == _value ) {
        Log.info("Dimmer " + _name + " setValue " + String(value) + " same so outputPWM without dimming");
        _currentLevel = convertPercent(_value);   // Turn 0-100 into 0x0 to 0x7fffffff
        outputPWM();
        return;
    }
    
    _currentLevel = convertPercent(_value);     // previous value
    _value = value;                             // percent 0-100
    _targetLevel = convertPercent(value);       // new 32 bit value
    Log.info("Dimmer " + _name + " setValue " + String(value) + " converted to " + String(_targetLevel));
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
void NCD16Light::startSmoothDimming() {
    if((int)_currentLevel != _targetLevel){
        _lastUpdateTime = millis();
        int delta = _targetLevel - _currentLevel;   // signed 32 bit
        _incrementPerMillisecond = delta / _dimmingDuration;
        if(_incrementPerMillisecond == 0) {
            if(delta > 0) {
                _incrementPerMillisecond = 1;
            } else {
                _incrementPerMillisecond = -1;
            }
        }
        Log.info("Dimmer "+_name+" setting increment to "+String(_incrementPerMillisecond)+", delta: "+String(delta));
    }
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void NCD16Light::loop()
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
void NCD16Light::outputPWM() {
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
        Log.error("NCD16Light outputPWM write failed for light "+String(_lightNum)+", level = "+String(current4k));
        reset();
    }
}

/**
 * Convert 0-100 to 0-0x0FFF (exponential) scale (0 - 4095)
 * 0 = 0, 100 = 0x0FFF
 */
int16_t scaledValues[] =
{
    //55, // 80: 29,196,319 0x1bd 801f >> 19 = 0x37 (55)
    //69, // 81: 36,194,676 0x0228 4974 >> 19 = 0x045 (69)
    85, // 82: 44,870,540 0x02ac ab8c >> 19 = 0x055 (85)
    106, // 83: 55,626,008 0x0350 c918 >> 19 = 0x06a (106)
    131, // 84: 68,959,563 0x041c 3d4b >> 19 = 0x083 (131)
    163, // 85: 85,489,170 0x0518 7612 >> 19 = 0x0a3 (163)
    202, // 86: 105,980,924 0x0651 23fc >> 19 = 0x0ca (202)
    250, // 87: 131,384,552 0x07d4 c4e8 >> 19 = 0x0fa (250)
    310, // 88: 162,877,429 0x09b5 4ff5 >> 19 = 0x136 (310)
    385, // 89: 201,919,148 0x0c09 0aac >> 19 = 0x181 (385)
    
    477, // 90: 250,319,169 0xeeb 9141 >> 19 = 0x1dd (477)
    591, // 91: 310,320,674 0x127f 1e22 >> 19 = 0x24f (591)
    733, // 92: 384,704,539 0x16ee 201b >> 19 = 0x02dd (733)
    909, // 93: 476,918,217 0x1c6d31c9 >> 19 = 0x038d (909)
    1127, // 94: 591,235,514 0x233d89ba >> 19 = 0x0467 (1,127)
    1398, // 95: 732,954,667 0x2bb0002b >> 19 = 0x576 (1,398)
    1733, // 96: 908,643,900 0x3628 ce3c >> 19 = 0x6c5 (1,733)
    2148, // 97: 1,126,445,843 0x4324 3313 >> 19 = 0x864 (2,148)
    2663, // 98: 1,396,454,912 0x533c 3600 >> 19 = 0xa67 (2,663)
    3302 // 99: 1,731,185,155 0x672f ca03 >> 19 = 0xce6 (3,302)
    // 100 = 2,146,150,236 0x7feb a75c >> 19 = 0xffd
};

// Calculate a somewhat exponential scale, but ensure each value
// is greater than the previous.
// So expValue = 1.2397 ^ value
int NCD16Light::convertPercent(int percent) {
    if (percent <= 0) return 0;
    if (percent < 82) return percent << 19;     // 82 is crossover point
    if (percent >= 100) return 0x7fffffff;

    // Apply exponential curve at or above 82
    return scaledValues[percent-82] << 19;
}
