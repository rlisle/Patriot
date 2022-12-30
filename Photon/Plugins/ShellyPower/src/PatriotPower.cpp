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

/**
 * Constructor
 * @param name is the name used to address the value
 * @param room is the room where it is located
 */
Power::Power(String name, String room)
    : Device(name, room)
{
    _type         = 'W';
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
    if(lcTopic.startsWith("shellies/em/emeter/")) {
        Log.info("Parser received: " + lcTopic + ", " + lcMessage);
        // Parse remainder of message after "shellies/em/emeter/"
        parsePowerMessage(topic.substring(19), message);
}
    
// Private Helper Methods
    
// This handles only messages like shellies/em/emeter/+/power #.# where + == 0 or 1
void MQTTManager::parsePowerMessage(String lcTopicEnd, String lcMessage)
{
    int lineNum;
    float fValue;
    int amps = 0;

    if(lcTopicEnd == "0/power") {
        lineNum = 0;
    } else if(lcTopicEnd == "1/power") {
        lineNum = 1;
    } else {
        return;
    }

    //TODO: decode floating point message and expose as a device
    fValue = lcMessage.toFloat();
    if(fValue > 0.0 && fValue < 6000.0) {
        Log.info("Power line %d = %f",lineNum,fValue);
        _powerUsage[lineNum] = fValue;
        if(lineNum == 1) {
            amps = int((_powerUsage[0] + _powerUsage[1]) / 120);
            publish(kPublishName + "/amps/position", String(amps));
        }
    }
}


    
/**
 * notify()
 * Publish switch state
 */
void Power::notify()
{
    String message = String(_value);
    
    IoT::publishMQTT(_name, message);
}

