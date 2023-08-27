/**
 light LED dimming control

 Features:
 - Smooth dimming with duration

 Leverage floating point since Photon2 has hardware FP support.
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "IoT.h"
//#include "math.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, String room, bool isInverted, bool forceDigital)
        : Device(name, room),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital)
{
    _targetValue             = 0.0;
    _floatValue            = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime          = 0;
    _type                    = 'L';
}

void Light::begin() {
    _dimmingDurationMsecs = isPwmSupported() ? 2000 : 0;
    pinMode(_pin, OUTPUT);
    
    int resolution = analogWriteResolution(_pin, 16);
    _percentMaxValue = (1 << ((resolution-1) - 1)) / 100.0;
    Log.info("Begin _percentMaxValue = %f",_percentMaxValue);
    
    outputPWM();
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void Light::setValue(int value) 
{
    if(_targetValue == value) {
        outputPWM();
        return;
    }
    _targetValue = value;
    if(_dimmingDurationMsecs == 0) {
        _value = value;
        outputPWM();
    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use floatValue to smoothly transition
 */
void Light::startSmoothDimming() {
    Log.info("startSmoothDimming");
    _lastUpdateTime = millis();         // Starting now
    _floatValue = (float)_value;        // Starting point
    Log.info("Initial value = %f",_floatValue);
    float delta = (float)_targetValue - _floatValue;    // 0-100 scale
    Log.info("Delta = %f",delta);
    _incrementPerMillisecond = delta / (float)_dimmingDurationMsecs;
    Log.info("Increment per msec = %f",_incrementPerMillisecond);
}

/**
 * Private Methods
 */

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 */
void Light::loop()
{
    // Is any fading transition done?
    if(_value == _targetValue) {
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _floatValue += _incrementPerMillisecond * millisSinceLastUpdate;
    _value = (int)_floatValue;
    if(_incrementPerMillisecond > 0) {
        if(_floatValue > _targetValue) {
            _value = _targetValue;
        }
    } else {
        if(_floatValue < _targetValue) {
            _value = _targetValue;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        Log.info("outputPWM %x",(int)_floatValue);
        int pwm = scalePWM(_floatValue);
        analogWrite(_pin, pwm);
    } else {
        bool isOn = _value > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, scalePWM(isHigh ? HIGH : LOW));
    }
}

/**
 * Convert 0-100 to 0-0x0000ffff (16 bit) exponential scale
 * 0 = 0, 100 = 0x0000ffff
 */
int Light::scalePWM(float value) {
    Log.info("percentMaxValue = %f",_percentMaxValue);
    float outputValue = value * _percentMaxValue;
    Log.info("outputValue = %f",outputValue);
    int intValue = (int)outputValue;
    Log.info("scaleValue = %d",intValue);
    return intValue;
    
////    float base = 1.05697667;        // 255
//    float base = 1.08673221;          // 4095
//    float pwm = pow(base,value);
//    if (pwm > 4095) {
//        return(4095);
//    }
//    return (int) pwm;
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported()
{
    switch(_pin) {
        case D1:
        case D15:
        case D17:
        case A2:
        case A5:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}
