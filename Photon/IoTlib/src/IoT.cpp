/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#include "IoT.h"

// Static Variables
Device*      Device::_devices = NULL;
MQTTManager* IoT::_mqttManager = NULL;

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void IoT::begin(String brokerIP, String controllerName)
{
    handleDaylightSavings();
    
    String connectID = controllerName + "Id";
    _mqttManager = new MQTTManager(brokerIP, connectID, controllerName);
    
    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe(kPublishName, IoT::subscribeHandler, MY_DEVICES);

    // Expose particle.io variables
    Device::expose();
    
}

// Photon library doesn't automatically support DST.
// TODO: improve this algorithm to work beyond next couple years
// from 2nd Sunday of March through 1st Sunday of November
// 2022 3/13 - 11/6, 2023 3/12 - 11/5, 2024 3/10 - 11/3
void IoT::handleDaylightSavings() {
    int month = Time.month();
    if(month > 3 && month < 11) {
        Time.beginDST();
    } else if(month == 3) {
        handleDSTMarch();
    } else if(month == 11) {
        handleDSTNovember();
    }
}

void IoT::handleDSTMarch() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day <= 7) return;
    
    switch(weekday) {
        case 1:     // Sunday
            if(day == 8 && hour < 2) return;
            break;
        case 2:
            if(day < 9) return;
        case 3:
            if(day < 10) return;
        case 4:
            if(day < 11) return;
        case 5:
            if(day < 12) return;
        case 6:
            if(day < 13) return;
        case 7:     // Saturday
        default:
            if(day < 14) return;
    }
    Time.beginDST();
}

void IoT::handleDSTNovember() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day > 7) return;
    
    switch(weekday) {
        case 1:     // Sunday
            if(day == 1 && hour >= 2) return;
            break;
        case 2:
            if(day > 2) return;
            break;
        case 3:
            if(day > 3) return;
            break;
        case 4:
            if(day > 4) return;
            break;
        case 5:
            if(day > 5) return;
            break;
        case 6:
            if(day > 6) return;
            break;
        case 7:     // Saturday
        default:
            if(day > 7) return;
    }
    Time.beginDST();
}

void IoT::mqttPublish(String topic, String message)
{
    if (_mqttManager != NULL) {
        _mqttManager->publish(topic, message);
    }
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    Device::loopAll();

    if (_mqttManager != NULL) {
        _mqttManager->loop();
    }

//    int day = Time.day();
//    if (Time.day() != _prevDay) {
        // Handle any once-a-day things
        // TODO:
        //if (Time.month() == 3 && )
        
//    }
}

/**
 * Particle.io Subscribe Handler
 * t:patriot m:<device>:<value>
 * This method handles commands from Alexa
 */
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data = String(rawData).trim();
    String event(eventName);
    
    Log.info("Particle.io subscribe received data: '"+event+"', '"+data+"'");
    
    if (_mqttManager != NULL) {
        _mqttManager->parseMessage(event.toLowerCase(), data.toLowerCase());
    }
}

/**
 MQTT Subscribe Handler
*/
void IoT::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    if(_mqttManager != NULL) {
        _mqttManager->mqttHandler(rawTopic, payload, length);
    }
}

/**
 publishValue()
 param: name of state
 param: value to assign state
 return: 0 success, -1 MQTT error
 */
int IoT::publishValue(String name, int value) {
    if(_mqttManager != NULL) {
        _mqttManager->publish("patriot/" + name, String(value));
        return 0;
    }
    return -1;
}
