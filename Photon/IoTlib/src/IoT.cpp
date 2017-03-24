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
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-24: Initial version
******************************************************************/
#include "IoT.h"
#include "light.h"

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
}

/**
 * Constructor.
 */
IoT::IoT()
{
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _hasBegun       = false;
    _publishName    = kDefaultPublishName;
    _controllerName = kDefaultControllerName;
    _presence = NULL;
    _proximity = NULL;
    _switches = NULL;       // Lazy loaded
    _temperature = NULL;
    _numSupportedActivities = 0;
}

/**
 * Configuration methods
 */
 void IoT::setControllerName(String controllerName)
{
    this->_controllerName = controllerName;
    if(_alive != NULL) {
        _alive->setControllerName(controllerName);
    }
}

void IoT::setPublishName(String publishName)
{
    this->_publishName = publishName;
    if(_alive != NULL) {
        _alive->setPublishName(publishName);
    }
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
    Serial.println(_controllerName+" controller starting...");

    _activities = new Activities();
    _alive = new Alive();
    _alive->setControllerName(_controllerName);
    _alive->setPublishName(_publishName);
    _controllerNames = new ControllerNames();
    _devices = new Devices();
    _deviceNames = new DeviceNames();

    Particle.subscribe(_publishName,globalSubscribeHandler);
    if(!Particle.variable(kSupportedActivitiesVariableName, supportedActivitiesVariable))
    {
        Serial.println("Unable to expose "+String(kSupportedActivitiesVariableName)+" variable");
    }

}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    if(!_hasBegun) return;

    _alive->loop();
    _devices->loop();
    if(_presence != NULL && _proximity != NULL) {
        _presence->loop(_proximity);
    }

    if(_switches != NULL) {
        _switches->loop();
    }

    if(_temperature != NULL) {
        _temperature->loop();
    }
}

//TODO: Move to behavior
// Proximity (Note: currently only 1 proxmity sensor at a time supported)
void IoT::monitorPresence(int triggerPin, int echoPin, int min, int max, String event)
{
    Serial.println("Monitoring presence on trigger pin "+String(triggerPin)+", echo pin "+String(echoPin));
    if(_proximity != NULL) {
        delete _proximity;
        _proximity = NULL;
    }
    if(_presence != NULL) {
        delete _presence;
        _presence = NULL;
    }
    _proximity = new Proximity(triggerPin, echoPin);
    _presence = new Presence(min, max, event, kPingInterval);
}

// Temperature
void IoT::monitorTemperature(int pin, int type, String msg, long interval)
{
    if(_temperature == NULL) {
        _temperature = new Temperature(pin, type);
    }
    _temperature->setText(msg);
    if(interval > 0) {
        _temperature->setInterval(interval);
    }
}


// Fan
void IoT::addFan(int pinNum, String name)
{
    Fan* fan = new Fan(pinNum, name);
    _devices->addDevice(fan);
    _deviceNames->addDevice(name+":fan");
}

// Lights
void IoT::addLight(int pin, String name)
{
    Light* light = new Light(pin, name);
    _devices->addDevice(light);
    _deviceNames->addDevice(name+":light");
}

// Switches
void IoT::addSwitch(int pin, String eventName)
{
    if(_switches == NULL) {
        _switches = new Switches();
    }
    _switches->addSwitch(pin, eventName);
}


// Activities
//TODO: collect array of all supported behaviors
void IoT::addBehavior(String deviceName, Behavior *behavior)
{
    Serial.println("addBehavior: "+deviceName);
    // Note: devices must be created before behaviors
    Device *device = _devices->getDeviceWithName(deviceName);
    if(device != NULL) {
        device->addBehavior(behavior);
        addToListOfSupportedActivities(behavior->activityName);
    } else {
        Particle.publish("ERROR", "IoT::addBehavior undefined device");
        Serial.println("Error: IoT addBehavior undefined device");
    }
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
            Serial.println("Supported activities = "+newVariable);
            supportedActivitiesVariable = newVariable;
        }
    } else {
        Serial.println("Supported activities variable is too long. Need to extend to a 2nd variable");
    }
}

/******************************************************/
/*** Expose variables listing devices and activities **/
/******************************************************/
bool IoT::exposeActivities()
{
    return _activities->expose();
}

bool IoT::exposeControllers()
{
    log("Expose controllers");
    _controllerNames->addController(_controllerName);
    return _controllerNames->expose();
}


/*************************/
/*** Subscribe Handler ***/
/*************************/
void IoT::subscribeHandler(const char *eventName, const char *rawData) {

    String data(rawData);   // This apparently converts the data somehow
    int colonPosition = data.indexOf(':');
    String name = data.substring(0,colonPosition);
    String state = data.substring(colonPosition+1);

    log("IoT subscribe: received "+name+":"+state);

    // Is a device coming online? (eg. ""<devicename>:Alive")
    // Is this an alive message?
    if(state.equalsIgnoreCase("alive"))
    {
        Serial.println("   alive");
        _controllerNames->addController(name);
        return;
    }

    // //TODO: Deprecate direct device commands
    // //      Instead, update current behavior
    // Device* device = _devices->getDeviceWithName(name);
    // if(device)
    // {
    //   int percent = device->convertCommandToPercent(state);
    //   Serial.println(" percent = "+String(percent));
    //   device->setPercent(percent);
    //   device->performActivities(_activities);
    //   return;
    // }
    //
    //TODO: maintain a list of supported activities, and
    //      search it like devices
    // If not, must be an activity/event name
    int value = state.toInt();
    Serial.println("   value = "+String(value));
    _activities->addActivity(name, value);
    Serial.println("   calling performActivities");
    _devices->performActivities(_activities);
    Serial.println("   subscript done");
}
