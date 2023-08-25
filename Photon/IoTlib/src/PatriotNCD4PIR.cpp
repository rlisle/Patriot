//
//  PatriotNCD4PIR.c
//  Patriot
//
//  Created by Ron Lisle on 8/24/23.
//

#include "IoT.h"

#define PIR_POLL_INTERVAL_MILLIS 1000

NCD4PIR::NCD4PIR(int8_t gpioIndex, String name, String room, int timeoutSecs) : Device(name, room), _timeoutMSecs(timeoutSecs * 1000) {
    _lastPollTime = 0;
    _lastMotion   = 0;
    _type         = 'M';    // Motion
    
    if(gpioIndex > 0 && gpioIndex <= 3) {
        _gpioBitmap = 0x10 << gpioIndex;
    } else {
        _gpioBitmap = 0x10;   // If 0 or invalid, set to first gpio
    }
}
void NCD4PIR::begin() {
    // Nothing to do here.
    // initializePCA9634 does all this once instead of for every device
}
bool NCD4PIR::isSensorOn() {
    return PCA9634::isInputOn(_gpioBitmap);
}
void NCD4PIR::reset() {
    Log.error("Resetting board");
    PCA9634::reset();
}
void NCD4PIR::loop() {
    if (isTimeToCheckSensor())
    {
        if (didSensorChange())
        {
            notify();
        }
    }
}
bool NCD4PIR::isTimeToCheckSensor() {
    long currentTime = millis();
    if (currentTime < _lastPollTime + PIR_POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}
bool NCD4PIR::didSensorChange() {
    //We shouldn't need to filter or debounce
    int newValue = isSensorOn() ? 100 : 0;
    if(newValue != _value) {
        _value = newValue;
        return true;
    }
    return false;
}
void NCD4PIR::notify() {
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}
