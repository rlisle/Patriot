/**
 NCD PCA9634 8-channel PWM board control

 Up to 8 I2C boards can reside on a single I2C bus.
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified

 Requires PCA9634 be installed and running also.
 Note that address switches on board have no offset.
        Address is as-is.
 
 Photon2 has native floating point, so no need to use integer math scaled to 7fffffff.
 All I2C code is in pca9685 class.

 PCA9634 D/A resolution is 8 bits
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:
     PCA9634: https://www.nxp.com/docs/en/data-sheet/PCA9634.pdf
 */

#include "IoT.h"
#include "math.h"

//#define CURVE_BASE 1.056976670749058    // pow(255.0, 1.0 / 100.0)
#define CURVE_BASE 1.05701804056138     // pow(256.0, 1.0 / 100.0)
// ^0 = 1, ^1 = 1.057, ^100 = 256 so subtract 1 from result for 0-255

/**
 * Constructor
 * @param lightNum is the channel number on the NCD 8 Light board (1-8)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */
NCD8Light::NCD8Light(int8_t lightNum, String name, String room, int8_t duration, int8_t curve)
                     : Device(name, room)
{
    _lightNum       = lightNum-1;       // Convert to 0 based
    _dimmingMSecs   = duration * 1000;
    _curve          = curve;
    _value          = 0;                // Base Device class
    _currentLevel   = 0.0;              // These 2 used to perform dimming
    _targetLevel    = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime = 0;
    _type           = 'L';
}

void NCD8Light::begin() {
    // Initialization done by PCA9634
}

void NCD8Light::reset() {
    Log.error("NCD8Light reset");
    PCA9634::reset();
}

/**
 * Set value
 * @param value Int 0 to 100.
 */
void NCD8Light::setValue(int value) {
    if( value == _value ) {
        outputPWM();
        return;
    }
    
    _targetLevel = (float)value;        // new value
    if(_dimmingMSecs == 0) {
        _currentLevel = _targetLevel;
        _value = value;
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
void NCD8Light::startSmoothDimming() {
    if(abs(_currentLevel - _targetLevel) > 0.001) { // if !=
        _lastUpdateTime = millis();
        float delta = _targetLevel - _currentLevel;
        _incrementPerMillisecond = delta / _dimmingMSecs;
        if(abs(_incrementPerMillisecond) < 0.001) {
            if(delta > 0) {
                _incrementPerMillisecond = 0.001;
            } else {
                _incrementPerMillisecond = -0.001;
            }
        }
    }
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void NCD8Light::loop()
{
    // Is dimming transition underway?
    if(abs(_currentLevel - _targetLevel) < 0.001) { // if ==
        return;
    }

    //TODO: millis will wrap after 49 days
    unsigned long loopTime = millis();
    unsigned long millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentLevel += _incrementPerMillisecond * millisSinceLastUpdate;
    
    if(_incrementPerMillisecond > 0) {  // Going up?
        if(_currentLevel > _targetLevel) {
            _currentLevel = _targetLevel;
        }
    } else {                            // Going down
        if(_currentLevel < _targetLevel) {
            _currentLevel = _targetLevel;
        }
    }
    
    // Clamp value
    _currentLevel = constrain(_currentLevel, 0.0, 100.0);
    _value = (int)_currentLevel;
    _lastUpdateTime = loopTime;

    outputPWM();
};

/**
 * Set the output PWM to 0-255 (8 bits)
 * Convert float 0-100 to 0-0x00ff
 */
void NCD8Light::outputPWM() {
    int current255 = convertTo255(_currentLevel);
//    Log.info("outputPWM light %d, level %d", _lightNum, current255);
    PCA9634::outputPWM(_lightNum, current255);
}


/**
 Convert 0 => 0, 100 => 255 with exponential scale
 */
int NCD8Light::convertTo255(int value) {
    if(value == 0) return 0;
    float expValue = pow(CURVE_BASE, float(value)) - 1.0;
    float linValue = (float)value * 255 / 100;

    // Curve argument specifies curve shape
    // 0 = Linear
    // 1 = Exponential
    // 2 = 50/50 split
    // 3 = 75/25 split
    if(_curve == 0) {           // Linear
        return linValue;
    } else if(_curve == 1) {    // Exponential (redundant)
        return expValue;
    }
    // Else return 2 = 1/2 + 1/2, 3 = 1/3 + 2/3, 4 = 1/4 + 3/4
    float expAmount = 1.0 / _curve;
    float linAmount = (_curve - 1) / _curve;
    float linearPart = linValue * linAmount;
    float exponentialPart = expValue * expAmount;
    float combinedValue = constrain(exponentialPart + linearPart, 0.0, 255.0);
    return round(combinedValue);
}
