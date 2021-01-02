/**
 NCD PCA9634 8-channel PWM board control

 Up to 8 I2C boards can reside on a single I2C bus.
 
 Features:
 - PWM control
 - Smooth transitioning if duration specified

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "PatriotNCD8Light.h"
#include "math.h"

#define MILLIS_PER_SECOND 1000

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7) 0x01 if low switch set, 0x40 if high
 * @param lightNum is the channel number on the NCD 8 Light board (0-7)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD8Light::NCD8Light(int8_t address, int8_t lightNum, String name, int8_t duration)
                     : Device(name)
{
    _address = address;
    _lightNum   = lightNum;
    _dimmingDuration = duration;
    _value = 0;
    _currentLevel = 0.0;
    _targetLevel = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime = 0;
}

void NCD8Light::begin() {
    initializeBoard();
}

int8_t NCD8Light::initializeBoard() {
    int status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    Wire.beginTransmission(_address);   // Seems unnecessary
    status = Wire.endTransmission();

    if(status == 0) {
        Wire.beginTransmission(_address);
        Wire.write(0);          // Control register - No AI, point to reg0 Mode1
        Wire.write(0);          // Mode1 reg. Osc on, disable AI, subaddrs, allcall
        Wire.endTransmission();

        Wire.beginTransmission(_address);
        Wire.write(1);          // Mode2 register
        Wire.write(0x04);       // Dimming, Not inverted, totem-pole
        Wire.endTransmission();

        Wire.beginTransmission(_address);
	    Wire.write(0x8c);       // AI + LEDOUT0
	    Wire.write(0xaa);       // LEDOUT0 LEDs 0-3 dimming 
	    Wire.write(0xaa);       // LEDOUT1 LEDS 4-7 dimming
        Wire.endTransmission();

        outputPWM();            // Force light off
        
        Log.info("InitializeBoard " + _name + " sucess");
        
    } else {
        Log.error("InitializeBoard " + _name + " FAILED!");
    }

    return status;
}

void NCD8Light::reset() {
    // Issue PCA9634 SWRST
    Wire.beginTransmission(_address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD8Light reset write failed for light "+String(_lightNum));
        return;
    }
    initializeBoard();
}

/**
 * Set value
 * @param value Int 0 to 100.
 */
void NCD8Light::setValue(int value) {
    _currentLevel = scalePWM(_value);
    _value = value;
    _targetLevel = scalePWM(value);
    Log.info("Dimmer " + String(_name) + " setValue " + String(value) + " scaled to " + String(_targetLevel));
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
void NCD8Light::startSmoothDimming() {
    if((int)_currentLevel != _targetLevel){
        _lastUpdateTime = millis();
        float delta = _targetLevel - _currentLevel;
        _incrementPerMillisecond = delta / (float(_dimmingDuration) * 1000);
        Log.info("Light "+_name+" setting increment to "+String(_incrementPerMillisecond));
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
    // Is fading transition underway?
    if(_currentLevel == _targetLevel) {
        return;
    }
    
    //Log.trace("light loop value: "+String(_value)+", target: "+String(_targetValue));

    // _currentLevel, _targetLevel, and _incrementPerMillisend are floats for smoother transitioning
    
    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentLevel += _incrementPerMillisecond * millisSinceLastUpdate;
    if(_incrementPerMillisecond > 0.0) {
        if(_currentLevel > _targetLevel) {
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
 * Set the output PWM _currentLevel (0-255)
 */
void NCD8Light::outputPWM() {
    int reg = 2 + _lightNum;
    
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(_address);
        Wire.write(reg);
        Wire.write(int(_currentLevel));
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0){
        Log.error("NCD8Light outputPWM write failed for light "+String(_lightNum)+", level = "+String(_currentLevel));
        Log.error("Resetting board");
        reset();
    }
}

/**
 * Convert 0-100 to 0-255 exponential scale
 * 0 = 0, 100 = 255
 */
float NCD8Light::scalePWM(int value) {
    if (value <= 0) return 0;
    if (value >= 100) return 255;
    
    //TODO: This is too extreme. Need to refine algorithm
    float base = 1.05697667;
    float pwm = pow(base,value);
    if (pwm > 255) {
        return(255);
    }
    return pwm;
}
