/******************************************************************
 IoT

 This is a static (singleton) class used in each IoT
 Arduino module sketch. It provides the main API for
 adding and configuring all the various devices.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-24: Initial version
 ******************************************************************/

#include "LisleRV.h"
#include "iot.h"
#include "light.h"

void globalSubscribeHandler(const char *eventName, const char *rawData) {
  IoT* iot = IoT::getInstance();
  iot->subscribeHandler(eventName,rawData);
}

IoT* IoT::_instance = NULL;

IoT* IoT::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new IoT();
    }
    return _instance;
}

IoT::IoT()
{
  _activities = new Activities();
  _alive = NULL;
  _controllerNames = new ControllerNames();
  _devices = new Devices();
  _deviceNames = new DeviceNames();
  _presence = NULL;
  _proximity = NULL;
  _switches = NULL;       // Lazy loaded
  _temperature = NULL;

  Particle.subscribe("lislerv",globalSubscribeHandler);
}

void IoT::start(String name)
{
  _name = name;

  Serial.begin(57600);
  Serial.println(name+" controller starting...");

  if(_alive == NULL) {
    _alive = new Alive(name);
  }
}

// Loop
void IoT::loop()
{
  if(_alive != NULL) {
    _alive->loop();
  }

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
void IoT::addFan(String name, int pinNum, int lowTemp, int highTemp, String event)
{
  Fan* fan = new Fan(name, pinNum);
  fan->addBehavior( new Behavior(event, '>', lowTemp, 25) );
  fan->addBehavior( new Behavior(event, '>', highTemp, 100) );
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
void IoT::addBehavior(String deviceName, Behavior *behavior)
{
  // Note: devices must be created before behaviors
  Device *device = _devices->getDeviceWithName(deviceName);
  if(device != NULL) {
    device->addBehavior(behavior);
  } else {
    Particle.publish("ERROR", "IoT::addBehavior undefined device");
    Serial.println("Error: IoT addBehavior undefined device");
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
  Serial.println("Expose controllers");
  _controllerNames->addController(_name);
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

  Serial.print("IoT subscribe: received "+name+":"+state+" -- ");

  // Is a device coming online? (eg. ""<devicename>:Alive")
  // Is this an alive message?
  if(state.equalsIgnoreCase("alive"))
  {
    Serial.println("   alive");
    _controllerNames->addController(name);
    return;
  }

  Device* device = _devices->getDeviceWithName(name);
  if(device)
  {
    int percent = device->convertCommandToPercent(state);
    Serial.println(" percent = "+String(percent));
    device->setPercent(percent);
    device->performActivities(_activities);
    return;
  }

  // If not, must be an activity/event name
  int value = state.toInt();
  Serial.println(" value = "+String(value));
  _activities->addActivity(name, value);
  _devices->performActivities(_activities);
}
