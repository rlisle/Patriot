/**
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

void globalLog(String message, PLogLevel logLevel = LogError) {
    IoT* iot = IoT::getInstance();
    iot->log(message, logLevel);
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
void IoT::log(String msg, PLogLevel logLevel)
{
    if (_logLevel == LogNone) return;
    if (_logLevel == LogError && logLevel != LogError) return;
    if (_logLevel == LogInfo && logLevel == LogDebug) return;
    
    Serial.println(msg);    // Is this needed?

    if (_mqttManager != NULL) {
        _mqttManager->publish(kPublishName+"/log", _controllerName + ": " + msg);
    }
}

/**
 * Set the amount of logging desired.
 */
void IoT::setLogLevel(PLogLevel logLevel) {
    _logLevel = logLevel;
}

/**
 * Constructor.
 */
IoT::IoT()
{
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _logLevel               = LogError;
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
    _isBridge = isBridge;
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName, _devices);
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
    log("addDevice: " + device->name(), LogDebug);
    if(device->shouldAutoCreateBehavior()) {
        bool isDefault = true;
        Behavior *defaultBehavior = new Behavior(100, isDefault);
        defaultBehavior->addCondition(new Condition(device->name(), '>', 0));
        device->addBehavior(defaultBehavior);
    }
    
    _devices->addDevice(device);
    device->logPtr = globalLog;
    device->publishPtr = globalPublish;
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
        Serial.println("IoT received unexpected particle.io topic: " + event);
        return;
    }
    
    // Legacy commands will include a colon
    // t:patriot m:<eventName>:<msg>
    int colonPosition = data.indexOf(':');
    if(colonPosition == -1) {
        Serial.println("IoT received invalid particle message: " + data);
        return;
    }

    log("Particle.io subscribe received data: '"+data+"'");
    
    // Convert to new protocol
    // eg. t:patriot m:DeskLamp:100 -> t:patriot/desklamp m:100
    String name = data.substring(0,colonPosition).toLowerCase();
    String level = data.substring(colonPosition+1).toLowerCase();
    String topic = kPublishName + "/" + name;

    //TODO: May want to simply use _isBridge and not handle directly here
    //      That would break if MQTT broker goes down. Is that ok?
    //      I think our assumptions is that MQTT is always up, and
    //      this method stops working when there is no internet.
    
    //TODO: Is this needed if _isBridge is set (handled below)?
    //TODO: Invoke this only if MQTT is not working
//    if(_mqttManager != NULL) {
//        _mqttManager->parseMessage(topic,level);
//    }
    
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
