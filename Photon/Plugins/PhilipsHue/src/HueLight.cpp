/******************************************************************
 HueLight plugin

 Features:
 - Control Philips Hue light.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "HueLight.h"
#include "IoT.h"

/**
 * Constructor
 * @param name  String name of the checklist item
 */
HueLight::HueLight(String name, String room)
        : Device(name, room)
{
    _value = 0;
    _type  = 'L';
}

void HueLight::begin() {
    //TODO: maybe setup or check WiFi connectivity?
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void HueLight::loop()
{
    //Nothing to do?
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void HueLight::setValue(int value) {
    Log.info("HueLight " + _name + " setValue: " + String(value));
    if(_targetValue == value) {
        Log.info("HueLight value already set, but writing again!");
    }
    writeToHue();
}


// Private Helper Methods
void HueLight::writeToHue() {
    //TODO: Write to Hue via Wifi
    
}

/**
 * notify()
 * Publish switch state
 */
void HueLight::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
