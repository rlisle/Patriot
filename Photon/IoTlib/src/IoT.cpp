/**
 This is the primary class for the Patriot IoT library.
 
 It aggregates all the other classes, and provides
 a common API for adding and configuring devices.
 
 This class coordinates realtime events.
 If cloud enabled, it subscribes to Particle.io notifications, and
 distributes them to devices.
 
 To build this code:
 1. Update version number in library.properties
 2. "particle library upload"
 3. Add library to controller (if needed) or update version
 4. "particle library add iot"
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#include "IoT.h"

// Static Variables
Device*      Device::_devices = NULL;
MQTTManager* IoT::_mqttManager = NULL;
bool IoT::_cloudEnabled = false;

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 *  Network may not be connected yet.
 */
void IoT::begin(String brokerIP, String controllerName, bool enableCloud)
{
    _cloudEnabled = enableCloud;
 
    Time.zone(-6);
    
    WiFi.on();
    WiFi.connect();

    if(_cloudEnabled == true) {
//        Particle.connect();   // Not needed if SYSTEM_MODE(AUTOMATIC) (default)
        
        // Expose particle.io variables
        Device::expose();
        
        // Subscribe doesn't require a connection before calling.
        Particle.subscribe(kPublishName, IoT::subscribeHandler, MY_DEVICES);
    }

    String connectID = controllerName + "Id";
    _mqttManager = new MQTTManager(brokerIP, connectID, controllerName);

}

void IoT::mqttPublish(String topic, String message)
{
    _mqttManager->publish(topic, message);
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    Device::loopAll();
    
    _mqttManager->loop();
    
    dailyReset();
}

/**
 * Perform a reboot daily at 2:00 am
 * and it has been running at least 6 hours
 */
void IoT::dailyReset() {
    if(Time.hour() == 2 && System.uptime() > 60*60*6 ) {
        System.reset(RESET_NO_WAIT);
    }
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
    
    _mqttManager->parseMessage(event.toLowerCase(), data.toLowerCase());
}

/**
 MQTT Subscribe Handler
 */
void IoT::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {
    
    _mqttManager->mqttHandler(rawTopic, payload, length);
}

/**
 publishValue()
 param: name of state
 param: value to assign state
 return: 0 success, -1 MQTT error
 */
void IoT::publishValue(String name, int value) {
    _mqttManager->publish("patriot/" + name, String(value));
}
