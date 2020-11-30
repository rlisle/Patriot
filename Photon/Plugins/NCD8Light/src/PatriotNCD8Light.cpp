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
 2020-11-29: Add scaling
 2020-02-11: Initial creation based on PatriotNCD8Relay and PatriotLight
 2020-11-05: Update based on testing with NCD8LightTest
 ******************************************************************/

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
                     : Device(name, DeviceType::NCD8Light)
{
    _address = address;
    _lightNum   = lightNum;
    // _percent is left uninitialized if retained storage is used to pickup state from SRAM
    _percent = 0;
    _currentPercent = _percent;
    _dimmingDuration = float(duration);
    _targetPercent = _percent;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime = 0;
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

    } else {
        Serial.println("initializeBoard failed");
    }

    return status;
}

/**
 * Set percent
 * @param percent Int 0 to 100.
 */
void NCD8Light::setPercent(int percent) {
    log("Dimmer " + String(_name) + " setPercent " + String(percent), LogDebug);
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
void NCD8Light::startSmoothDimming() {
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
    
    log("light loop percent: "+String(_percent)+", target: "+String(_targetPercent), LogDebug);

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
void NCD8Light::outputPWM() {
    int val = scalePWM(_percent);
    int reg = 2 + _lightNum;
    Wire.beginTransmission(_address);
	Wire.write(reg);
	Wire.write(val);
	byte status = Wire.endTransmission();
	if(status != 0){
		Serial.println("outputPWM write failed");
	}
}

/**
 * Convert 0-100 percent to 0-255 exponential scale
 * 0 = 0, 100 = 255
 */
int NCD8Light::scalePWM(int percent) {
    if (percent <= 0) return 0;
    if (percent >= 100) return 255;
    
    float base = 1.05697667;
    float pwm = pow(base,percent);
    if (pwm > 255) {
        return(255);
    }
    return (int) pwm;
}
