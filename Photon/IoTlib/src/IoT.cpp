/******************************************************************
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices and states.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-24: Remove unused variables, refactor parsing
2020-11-22: Integrate DeviceNames into Devices
2020-11-21: Delete publishName, implement new MQTT protocol
2020-11-14: Rename activities to states. Delete supportedStates.
2019-01-05: v3.0.0 Removed watchdog timer due to OTA issues.
2019-01-01: Replace 2am reset with hardware watchdog timer.
2018-11-05: Refactor to MQTTmanager.
2018-10-15: Expose MQTT publish.
2018-09-04: Bridge Particle to MQTT
2018-07-07: Convert MQTT format to match SmartThings
2018-03-16: Add MQTT support
2018-01-17: Add functions for device state and type
2017-10-22: Convert to scene-like behavior
2017-10-12: Add control using device names
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-24: Initial version
******************************************************************/
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

void globalQOScallback(unsigned int data) {
    IoT* iot = IoT::getInstance();
    iot->mqttQOSHandler(data);
}

void globalLog(String message) {
    IoT* iot = IoT::getInstance();
    iot->log(message);
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
 * Helper log method
 * Simply passes msg along to Serial.println, but also provides
 * a spot to add more extensive logging or analytics
 * @param msg
 */
void IoT::log(String msg)
{
    Serial.println(msg);

    IoT* iot = IoT::getInstance();
    if(iot->_mqttManager) {
        iot->_mqttManager->log(msg);
    }
}

/**
 * Constructor.
 */
IoT::IoT()
{
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _factory                = new Factory();
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

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void IoT::begin()
{
    Serial.begin(57600);

    _devices = new Devices();

    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe(kPublishName, globalSubscribeHandler, MY_DEVICES);
}

// MQTT 
void IoT::connectMQTT(String brokerIP, String connectID, bool isBridge)
{
//    Serial.println("Connecting to MQTT patriot on IP " + brokerIP);
    _isBridge = isBridge;
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName, _devices);
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
    if(_devices != NULL) {
        _devices->loop();
    }

    if (_mqttManager != NULL) {
        _mqttManager->loop();
    }
}

// Add a Device
// 
void IoT::addDevice(Device *device)
{
    //TODO: Automatically create a direct device command behavior (unless it's a switch)
    if(device->shouldAutoCreateBehavior()) {
        Behavior *defaultBehavior = new Behavior(100);
        defaultBehavior->addCondition(new Condition(device->name(), '>', 0));
        device->addBehavior(defaultBehavior);
    }
    
    _devices->addDevice(device);
    device->log = globalLog;
    device->publish = globalPublish;
}


/*************************************/
/*** Particle.io Subscribe Handler ***/
/*** t:patriot m:<device>:<value>  ***/
/*************************************/
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data(rawData);
    String event(eventName);
//    Serial.println("Subscribe handler event: " + event + ", data: " + data);

    if(event.equalsIgnoreCase(kPublishName) == false) {
        Serial.println("IoT received unexpected particle.io topic: " + event);
        return;
    }
    
    // Legacy commands will include a colon
    // t:patriot m:<eventName>:<msg>
    // Convert to new protocol
    // eg. t:patriot m:DeskLamp:100 -> t:patriot/desklamp m:100
    int colonPosition = data.indexOf(':');
    if(colonPosition == -1) {
        Serial.println("IoT received invalid particle message: " + data);
        return;
    }

    String name = data.substring(0,colonPosition).toLowerCase();
    String level = data.substring(colonPosition+1).toLowerCase();
    String topic = kPublishName + "/" + name;

    //TODO: Is this needed if _isBridge is set (handled below)?
    if(_mqttManager != NULL) {
        _mqttManager->parseMessage(topic,level);
    }
    
    // Bridge events to MQTT if this is a Bridge
    if(_isBridge)
    {
      if (_mqttManager != NULL) {
          _mqttManager->publish(topic, level);
      }
    }
}

/******************************/
/*** MQTT Subscribe Handler ***/
/******************************/
void IoT::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    if(_mqttManager != NULL) {
        _mqttManager->mqttHandler(rawTopic, payload, length);
    }
}

void IoT::mqttQOSHandler(unsigned int data) {
    if(_mqttManager != NULL) {
        _mqttManager->mqttQOSHandler(data);
    }
}
