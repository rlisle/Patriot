/******************************************************************
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices and activities.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-03-16: Add MQTT support
2017-10-22: Convert to scene-like behavior
2017-10-12: Add control using device names
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-24: Initial version
******************************************************************/
#include "IoT.h"

void mqttHandler(char* topic, byte* payload, unsigned int length);

/**
 * Global subscribe handler
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
 * Supported Activities variable
 * This variable is updated to contain a comma separated list of
 * the activity names supported by this controller.
 * This allows applications to automatically determine activity names
 */
String supportedActivitiesVariable;

/**
 * Publish Name variable
 * This variable communicates the Particle.io publish event name.
 * This allows applications to automatically determine the event name
 * to use when publishing or subscribing to events to/from this device.
 *
 * It is also used by plugins when publishing events.
 *
 * Note: Do not change this or the Alexa and iOS apps my not work.
 *       This will be fixed in the future.
 */
String publishNameVariable;

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
    Particle.publish("LOG", msg, 60, PRIVATE);
}

/**
 * Constructor.
 */
IoT::IoT()
{
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _hasBegun               = false;
    publishNameVariable     = kDefaultPublishName;
    _numSupportedActivities = 0;
}

/**
 * This function is used to change the particle.io publish
 * event name. Currently the event name is hardcoded to
 * 'patriot' in the Alexa skills and iOS apps.
 * In the future they will determine this from the Photon.
 * Until then, do not use this function.
 *
 * @param publishName
 */
void IoT::setPublishName(String publishName)
{
    publishNameVariable = publishName;
}

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void IoT::begin()
{
    if(_hasBegun) return;
    _hasBegun = true;

    Serial.begin(57600);

    _activities = new Activities();
    _behaviors = new Behaviors();
    _devices = new Devices();
    _deviceNames = new DeviceNames();

    Particle.subscribe(publishNameVariable, globalSubscribeHandler, MY_DEVICES);
    if(!Particle.variable(kSupportedActivitiesVariableName, supportedActivitiesVariable))
    {
        log("Unable to expose "+String(kSupportedActivitiesVariableName)+" variable");
        return;
    }
    if(!Particle.variable(kPublishVariableName, publishNameVariable))
    {
        log("Unable to expose publishName variable");
        return;
    }
}

void IoT::connectMQTT(char *brokerIP)
{
    // Do we need to disconnect if previously connected?
    _mqtt = MQTT(brokerIP, 1883, callback);
    _mqtt.connect(publishNameVariable);  // argument ignored?
    if (_mqtt.isConnected()) {
        _mqtt.subscribe(publishNameVariable);
    }
}


/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    if(!_hasBegun) return;

    _devices->loop();
}


// Add a Device
void IoT::addDevice(Device *device)
{
    _devices->addDevice(device);
    if(device->name() != "") {
        Serial.println("IoT adding device: "+device->name()+".");
        _deviceNames->addDevice(device->name());
    }
    else
    {
        Serial.println("IoT adding unnamed device. (Probably an input only device)");
    }
}


// Activities
void IoT::addBehavior(Behavior *behavior)
{
    _behaviors->addBehavior(behavior);
    addToListOfSupportedActivities(behavior->activityName);
}

void IoT::addToListOfSupportedActivities(String activity)
{
    for(int i=0; i<_numSupportedActivities; i++) {
        if(activity.equalsIgnoreCase(_supportedActivities[i])) return;
    }
    if(_numSupportedActivities < kMaxNumberActivities-1) {
        _supportedActivities[_numSupportedActivities++] = activity;
    }
    buildSupportedActivitiesVariable();
}

void IoT::buildSupportedActivitiesVariable()
{
    String newVariable = "";
    for(int i=0; i<_numSupportedActivities; i++)
    {
        newVariable += _supportedActivities[i];
        if (i < _numSupportedActivities-1) {
            newVariable += ",";
        }
    }
    if(newVariable.length() < kMaxVariableStringLength) {
        if(newVariable != supportedActivitiesVariable) {
            log("Supported activities = "+newVariable);
            supportedActivitiesVariable = newVariable;
        }
    } else {
        log("Supported activities variable is too long. Need to extend to a 2nd variable");
    }
}

/*************************************/
/*** Particle.io Subscribe Handler ***/
/*************************************/
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data(rawData);
    String event(eventName);
    Serial.println("Subscribe handler event: " + event + ", data: " + data);
    int colonPosition = data.indexOf(':');
    String name = data.substring(0,colonPosition);
    String state = data.substring(colonPosition+1);

    // See if this is a device name. If so, update it.
     Device* device = _devices->getDeviceWithName(name);
     if(device)
     {
       int percent = state.toInt();
       Serial.println(" percent = "+String(percent));
       device->setPercent(percent);
       return;
     }

    // If it wasn't a device name, it must be an activity.
    int value = state.toInt();
    log("   performing activity " + name + " = " + String(value));
    _behaviors->performActivity(name, value);
}

/******************************/
/*** MQTT Subscribe Handler ***/
/******************************/
void IoT::mqttHandler(char* topic, byte* payload, unsigned int length) {
//    char p[length + 1];
//    memcpy(p, payload, length);
//    p[length] = NULL;
    String data(payload);
    String event(topic);
    Serial.println("MQTT handler event: " + event + ", data: " + data);
    int colonPosition = data.indexOf(':');
    String name = data.substring(0,colonPosition);
    String state = data.substring(colonPosition+1);

    // See if this is a device name. If so, update it.
    Device* device = _devices->getDeviceWithName(name);
    if(device)
    {
        int percent = state.toInt();
        Serial.println(" percent = "+String(percent));
        device->setPercent(percent);
        return;
    }

    // If it wasn't a device name, it must be an activity.
    int value = state.toInt();
    log("   performing activity " + name + " = " + String(value));
    _behaviors->performActivity(name, value);
}
