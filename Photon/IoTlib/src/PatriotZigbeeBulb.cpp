/**
 Zigbee light bulb dimming control
 for use with Sonoff USB Zigbee adapter

 Features:
 - Smooth dimming with duration

 Leverage capability of Sengled 100w E11-N13 light bulb.
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "IoT.h"

/**
 * Constructor
 * @param name String name used to address the light.
 */
ZigbeeLight::ZigbeeLight(String name, String room, int durationSecs)
        : Device(name, room),
          _dimmingSecs(durationSecs)
{
    _type               = 'L';
}

void ZigbeeLight::begin() {
    // Nothing to do
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void ZigbeeLight::setValue(int value) 
{
    //Convert percent to 0-254
    float tempfValue = (float)value;
    float outputfValue = tempfValue * 2.54;
    String outputValue = String((int)outputfValue);
    String duration = String(_dimmingSecs);
    //TODO: output MQTT - IoT::publishMQTT(subtopic, message, retain=false)
    // patriot/zigbee/<name>/set -m {"brightness":128, “transition”:<_dimmingSecs>}
    String topic = "zigbee/" + _name + "/set";
    String brightness = "{\"brightness\":" + outputValue + ", ";
    String transition = "\"transition\": " + duration + "}";
    IoT::publishMQTT(topic, brightness + transition);
    _value = value;
}

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 * but this is done by the Sengled light bulb itself.
 */
void ZigbeeLight::loop()
{
    //Nothing to do
};
