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

/**
 * Global Particle.io subscribe handler
 * Called by particle.io when events are published.
 *
 * @param eventName
 * @param rawData
 */
void globalSubscribeHandler(const char *eventName, const char *rawData) {
    IoT* iot = IoT::getInstance();
    iot->subscribeHandler(eventName,rawData);
}

/**
 * Global MQTT subscribe handler
 * Called by MQTT when events are published.
 *
 * @param eventName
 * @param rawData
 */
void globalMQTTHandler(char *topic, byte* payload, unsigned int length) {
    IoT* iot = IoT::getInstance();
    iot->mqttHandler(topic, payload, length);
}

void globalPublish(String topic, String message) {
    IoT* iot = IoT::getInstance();
    iot->mqttPublish(topic, message);
}

/**
 * Singleton IoT instance
 * Use getInstance() instead of constructor
 */
IoT* IoT::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new IoT();
    }
    return _instance;
}
IoT* IoT::_instance = NULL;

/**
 * Constructor.
 */
IoT::IoT()
{
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _controllerName         = kDefaultControllerName;
    _mqttManager            = NULL;
}

/**
 * Specify the controller's name
 * 
 * @param controllerName
 */
void IoT::setControllerName(String name)
{
    _controllerName = name.toLowerCase();
}

Device* Device::_devices = NULL;

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void IoT::begin()
{
    //Use Log instead of Serial. MQTT will receive log messages.
    //If MQTT won't work, initialize SerialLogHandler logHandler(LOG_LEVEL_ALL);
    //Serial.begin(57600);

    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe(kPublishName, globalSubscribeHandler, MY_DEVICES);

    // Expose particle.io variables
    Device::expose();
    //Device::exposeStates();

}

// MQTT 
void IoT::connectMQTT(String brokerIP, String connectID)
{
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName);
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
    
    if(event.equalsIgnoreCase(kPublishName) == false) {
        Log.warn("IoT received unexpected particle.io topic: " + event);
        return;
    }
    
    // Legacy commands will include a colon
    // t:patriot m:<eventName>:<msg>
    int colonPosition = data.indexOf(':');
    if(colonPosition == -1) {
        Log.error("IoT received invalid particle message: " + data);
        return;
    }

    Log.info("Particle.io subscribe received data: '"+data+"'");
    
    String name = data.substring(0,colonPosition).toLowerCase();
    String level = data.substring(colonPosition+1).toLowerCase();
    String topic = kPublishName + "/" + name;

      if (_mqttManager != NULL) {
          _mqttManager->parseMessage(topic, level);
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
 Sketch Programming Support
 */

bool IoT::handleLightSwitch(String name) {
    Device *lightSwitch = Device::get(name+"Switch");
    if( lightSwitch == NULL) {
        Log.error("handleLightSwitch: " + name + "Switch not found!");
        return false;
    }
    if( lightSwitch->hasChanged() ) {
        lightSwitch->syncPrevious();
        Log.info("handleLightSwitch hasChanged");
        Device *light = Device::get(name);
        if( light == NULL ) {
            Log.error("handleLightSwitch: light " + name + " not found!");
            return false;
        }
        Log.info("Setting light to %d", lightSwitch->value());
        light->setValue( lightSwitch->value() );
        return true;
    }
    return false;
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
