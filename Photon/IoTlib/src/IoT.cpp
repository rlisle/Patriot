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
    _hasBegun               = false;
    _controllerName         = kDefaultControllerName;
    _mqttManager            = NULL;
    _mqttParser             = NULL;
    _startTime              = Time.now();
    _currentTime            = _startTime;
}

/**
 * Specify the controller's name
 * 
 * @param controllerName
 */
void IoT::setControllerName(String name)
{
    _controllerName = name;
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

    _devices = new Devices();
    _deviceNames = new DeviceNames();

    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe(kPublishName, globalSubscribeHandler, MY_DEVICES);
}

// MQTT 
void IoT::connectMQTT(String brokerIP, String connectID, bool isBridge)
{
//    Serial.println("Connecting to MQTT patriot on IP " + brokerIP);
    _isBridge = isBridge;
    _mqttParser = new MQTTParser(_controllerName, _devices);
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName, _mqttParser);
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
    _currentTime = Time.now();

    if(_devices != NULL) {
        _devices->loop();
    }

    if (_mqttManager != NULL) {
        _mqttManager->loop();
    }

    // Call tickle regularly to ensure the watchdogs do not reset
    //PhotonWdgs::tickle();  
}

// Add a Device
// 
void IoT::addDevice(Device *device)
{
    _devices->addDevice(device);
    if(device->name() != "") {
//        Serial.println("IoT adding device: "+device->name()+".");
        device->log = globalLog;
        device->publish = globalPublish;
        _deviceNames->addDevice(device->name());
    }
    else
    {
        Serial.println("IoT adding unnamed device. (Probably an input only device)");
    }
    //TODO: we could call buildSupportedStatesVariable here instead of requiring controllers to do it.
}


// States
//TODO: Update this to work with device based behaviors
// void IoT::addToListOfSupportedStates(String state)
// {
//     for(int i=0; i<_numSupportedStates; i++) {
//         if(state.equalsIgnoreCase(_supportedStates[i])) return;
//     }
//     if(_numSupportedStates < kMaxNumberStates-1) {
//         _supportedStates[_numSupportedStates++] = state;
//     }
//     buildSupportedStatesVariable();
// }

// void IoT::buildSupportedStatesVariable()
// {
//     String newVariable = "";
//     for(int i=0; i<_numSupportedStates; i++)
//     {
//         newVariable += _supportedStates[i];
//         if (i < _numSupportedStates-1) {
//             newVariable += ",";
//         }
//     }
//     if(newVariable.length() < kMaxVariableStringLength) {
//         if(newVariable != supportedStatesVariable) {
//             supportedStatesVariable = newVariable;
//         }
//     } else {
//         log("Supported states variable is too long. Need to extend to a 2nd variable");
//     }
// }

/*************************************/
/*** Particle.io Subscribe Handler ***/
/*** t:patriot m:<device>:<value>  ***/
/*** t:patriot m:mqtt-ip:<domain>  ***/
/*************************************/
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data(rawData);
    String event(eventName);
//    Serial.println("Subscribe handler event: " + event + ", data: " + data);

    // Bridge events to MQTT if this is a Bridge
    if(_isBridge)
    {
      if (_mqttManager != NULL) {
          _mqttManager->publish(event, data);
      }
    }

    // Legacy commands will include a colon
    // t:patriot m:<eventName>:<msg>
    // eg. t:patriot m:DeskLamp:100 -> t:patriot m:DeskLamp:100
    int colonPosition = data.indexOf(':');
    if(colonPosition == -1) return;

    String name = data.substring(0,colonPosition);
    String state = data.substring(colonPosition+1);

    // See if this is a device name. If so, update it.
    Device* device = _devices->getDeviceWithName(name);
    if(device)
    {
        int percent = state.toInt();
//        Serial.println(" percent = "+String(percent));
        //TODO: Instead, we need a behavior for this. Else lost on next state change.
        device->setPercent(percent);
        return;
    }

    // If it wasn't a device name, it must be an activity state.
    int value = state.toInt();
    _mqttParser->_states->addState(name,value);
    _devices->stateDidChange(_mqttParser->_states);
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

/**
 * Program Handler
 * !!!CURRENTLY NOT USED NOR TESTED!!!
 * Called by particle.io to update behaviors.
 * It will define a new behavior for a state for the specified device,
 * and return an int indicating if the state is new or changed.
 *
 * @param command "device:state:compare:value:level"
 * @returns int response indicating if state already existed (1) or error (-1)
 */
// int IoT::programHandler(String command) {
//     log("programHandler called with command: " + command);
//     String components[5];

//     int lastColonPosition = -1;
//     for(int i = 0; i < 4; i++)
//     {
//         int colonPosition = command.indexOf(':', lastColonPosition+1);
//         if(colonPosition == -1)
//         {
//             return -1 - i;
//         }
//         components[i] = command.substring(lastColonPosition+1, colonPosition);
//         lastColonPosition = colonPosition;
//     }
//     components[4] = command.substring(lastColonPosition+1);

//     // Parse out each item into the correct type
//     Device *device = _devices->getDeviceWithName(components[0]);
//     String state = components[1];
//     char compare = components[2].charAt(0);
//     int value = components[3].toInt();
//     int level = components[4].toInt();

//     //TODO: see if behavior already exists. If so, then change it.
//     //      Is there already a behavior for the same device and state?


//     //TODO: Otherwise just add a new behavior.
//     log("programHandler: new behavior("+components[0]+", "+components[1]+", "+components[2]+", "+components[3]+", "+components[4]+")");
//     addBehavior(new Behavior(device, state, compare, value, level));
//     addBehavior(new Behavior(device, state, '=', 0, 0));         // Add 'Off' state also
//     return 0;
// }

/**
 * Value Handler
 * Called by particle.io to read device current value.
 * It will return an int indicating the current value of the specified device.
 *
 * @param deviceName String name of device
 * @returns int response indicating value (0-100) or -1 if invalid device or error.
 */
// int IoT::valueHandler(String deviceName) {
//     Device *device = _devices->getDeviceWithName(deviceName);
//     if(device==NULL) {
//         return -1;
//     }
//     return device->getPercent();
// }

/**
 * Type Handler
 * Called by particle.io to read device type (enum).
 * It will return a string indicating the type of the specified device.
 * A string is used to allow flexibility and simple future expansion.
 *
 * @param deviceName String name of device
 * @returns int indicating DeviceType of device
 */
// int IoT::typeHandler(String deviceName) {
//     Device *device = _devices->getDeviceWithName(deviceName);
//     if(device==NULL) {
//         return -1;
//     }
//     return static_cast<int>(device->type());
// }
