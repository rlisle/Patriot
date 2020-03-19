/******************************************************************
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

 Changelog:
 2020-02-11: Initial creation based on PatriotNCD8Relay and PatriotLight
 ******************************************************************/

#include "PatriotNCD8Light.h"

#define MILLIS_PER_SECOND 1000

int8_t NCD8Light::_address;             // Addresses of board

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7) 0x01 if low switch set
 * @param lightNum is the channel number on the NCD 8 Light board (0-7)
 * @param name String name used to address the light.
 * @param duration Optional seconds value to transition. 0 = immediate, no transition.
 */

NCD8Light::NCD8Light(int8_t address, int8_t lightNum, String name, int8_t duration)
    : Device(name, DeviceType::NCD8Light)
{
    _lightNum   = lightNum;
    // _percent is left uninitialized to pickup state from SRAM
    _currentPercent = _percent;
    _brightness = 100;      // Default to 100
    _dimmingDuration = 2.0; // Default to 2 seconds
    _targetPercent = _percent;
    _incrementPerMillisecond = 0.0;
    _lastUpdatetime = 0;
    _duration   = duration;
    _address = address;
    initializeBoard();
}

int8_t NCD8Light::initializeBoard() {
    int retries = 3;
    byte status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    do {
        Wire.beginTransmission(_address);
        status = Wire.endTransmission();
    } while( status != 0 && retries-- > 0);

    if(status == 0) {
        Wire.beginTransmission(_address);
        Wire.write(0);          // Mode1 register
        Wire.write(0);          // Osc on, disable AI, subaddrs, allcall
        Wire.endTransmission();

        Wire.beginTransmission(_address);
        Wire.write(1);          // Mode2 register
        Wire.write(0x04);       // Dimming, Not inverted, totem-pole
        Wire.endTransmission();

    } else {
        Serial.println("initializeBoard failed");
    }

    return status;
}

/**
 * Set percent
 * @param percent Int 0 to 100.
 */
void NCD8Light::changePercent(int percent) {
    _targetPercent = percent;
    if(_dimmingDuration == 0.0) {
        _percent = percent;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use float _currentPercent value to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void Light::startSmoothDimming() {
    if((int)_percent != _targetPercent){
        _currentPercent = _percent;
        _lastUpdateTime = millis();
        float delta = _targetPercent - _percent;
        _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
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
    if(_percent == _targetPercent) {
        // Nothing to do.
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMillisecond * millisSinceLastUpdate;
    _percent = _currentPercent;
    if(_incrementPerMillisecond > 0) {
        if(_currentPercent > _targetPercent) {
            _percent = _targetPercent;
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _percent = _targetPercent;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 percent value
 */
void Light::outputPWM() {
    float pwm = _percent;
    pwm *= 255.0;
    pwm /= 100.0;
    int reg = 2 + _lightNum;
    Wire.beginTransmission(_address);
	Wire.write(reg);
	Wire.write( (int) pwm);
	byte status = Wire.endTransmission();
	if(status != 0){
		Serial.println("Write failed");
	}
}
