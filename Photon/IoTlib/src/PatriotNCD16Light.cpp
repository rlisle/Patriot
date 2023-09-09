/**
 NCD PCA9685 16-channel 12-Bit PWM board control

 Up to 8 I2C boards can reside on a single I2C bus. ???
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified
 
 - The "Common Bus" appears to be isolated from everything else, so can be used as
  as a + power strip for the LEDs. It can be sourced separately if desired.
 
 Photon2 has native floating point, so no need to use integer math scaled to 7fffffff.
 All I2C code is in pac9685 class.
 
 PCA9685 D/A resolution is 12 bits (0-0fff): 4096 steps
 
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

//TODOs:
//  Stagger on delays to reduce power surges

#include "IoT.h"
#include "math.h"

#define MILLIS_PER_SECOND 1000

// NOTE: Requires pca9685 also
/**
 * Constructor
 * @param address is the board address set by 6 jumpers: 0x01 if low switch set, 0x40 if high
 * @param lightNum is the channel number on the NCD 16 Dimmer board (1 - 16)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */
NCD16Light::NCD16Light(int8_t lightNum, String name, String room, int8_t duration)
                     : Device(name, room)
{
    _lightNum = constrain(lightNum-1, 0, 15);
    _dimmingMSecs = duration * 1000; // Convert to msecs
    _value = 0;             // Base Device class
    _currentLevel = 0.0;    // These 2 used to perform dimming
    _targetLevel = 0.0;     // "
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime = 0;
    _type = 'L';
}

void NCD16Light::begin() {
    // Initialization done by PCA9685
}

void NCD16Light::reset() {
    Log.error("NCD16Light reset");
    PCA9685::reset();
}

/**
 * Set value
 * @param value Int 0 to 100.
 */
void NCD16Light::setValue(int value) {
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
void NCD16Light::startSmoothDimming() {
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
void NCD16Light::loop()
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
 * Set the output PWM to 0-4095 (12 bits)
 * Convert float 0-100 to 0-0x7fff
 */
void NCD16Light::outputPWM() {
//    int current4k = (int)(_currentLevel * 4095.0 / 100.0);
    int current4k = convertTo4k(_currentLevel);
    
    //TODO: Apply an exponential-ish curve
    //      More lower level steps, fewer higher level steps
    PCA9685::outputPWM(_lightNum, current4k);
}

/**
 Convert 0 => 0, 100 => 4095 with exponential scale
 */
int NCD16Light::convertTo4k(int value) {
    //TODO: precalculate this
    float base = pow(4095.0, 1.0 / 100.0);
    float exponentialValue = pow(base, float(value));
    float linearValue = (float)value * 4095 / 100;
    // Using 50/50 split now, but could use any proportion
    float combinedValue = constrain(exponentialValue + linearValue / 2, 0.0, 4095.0);
    return int(combinedValue);
}
