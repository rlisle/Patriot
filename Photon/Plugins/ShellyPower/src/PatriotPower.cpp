/**
 Patriot Power Usage Monitoring

 This plugin supports monitoring of the power usage reported via MQTT by Shelly device
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:
 Shelly Power Monitor

 Changelog:
 2022-12-29: Initial creation
 */
#include "PatriotPower.h"
#include "IoT.h"
#include "constants.h"


/**
 * Constructor
 * @param name is the name used to address the value
 * @param room is the room where it is located
 */
Power::Power(String name, String room)
    : Device(name, room, 'W')
{
    _powerUsage[0] = 0.0;
    _powerUsage[1] = 0.0;
}

/**
 * loop()
 */
void Power::loop()
{
    //TODO:
}

void Power::mqtt(String topic, String message)
{
    int lineNum;
    float fValue;
    int newValue = _value;
    
    if(topic == "shellies/em/emeter/0/power") {
        lineNum = 0;
    } else if(topic == "shellies/em/emeter/1/power") {
        lineNum = 1;
    } else {
        return;
    }

    fValue = message.toFloat();
    if(fValue > 0.0 && fValue < 6000.0) {
        _powerUsage[lineNum] = fValue;
        if(lineNum == 1) {
            newValue = int((_powerUsage[0] + _powerUsage[1]) / 120);
            if(newValue != _value) {
                _value = newValue;
                notify();
            }
        }
    } else {
        Log.warn("power value out of range: " + String(fValue));
    }
}


    
/**
 * notify()
 * Publish switch state
 */
void Power::notify()
{
    String message = String(_value);
    IoT::publishMQTT("amps/position", message);
}

