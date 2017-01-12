/******************************************************************
 IoT

 This is the primary class for RvAutomation.
 It aggregates all the other classes, and provides
 a common API for adding and configuring devices.

 This class coordinates realtime events.
 It subscribes to Particle.io notifications, and
 distributes them to devices and activities.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-24: Initial version
 ******************************************************************/

#ifndef iot_h
#define iot_h

#include "LisleRV.h"
#include "alive.h"
#include "activities.h"
#include "controllernames.h"
#include "devices.h"
#include "devicenames.h"
#include "fan.h"
#include "proximity.h"
#include "presence.h"
#include "switches.h"
#include "temperature.h"

class IoT
{
  friend void globalSubscribeHandler(const char *eventName, const char *rawData);
  friend void globalDhtHandler();

private:
  static IoT  *_instance;
  String      _name;
  String      _proximityEvent;

  IoT();

  Alive       *_alive;
  Activities  *_activities;
  Devices     *_devices;
  DeviceNames *_deviceNames;
  ControllerNames *_controllerNames;
  Presence    *_presence;
  Proximity   *_proximity;
  Temperature *_temperature;
  Switches    *_switches;       // lazy loaded

  void subscribeHandler(const char *eventName, const char *rawData);
  void dhtHandler();
  void setupSerial();
  void setupAlive();

public:
  static IoT* getInstance();
  void start(String name);

  void monitorPresence(int triggerPin, int sensorPin, int min, int max, String event);
  void monitorTemperature(int pin, int type, String msg, long interval);
  bool exposeControllers();
  bool exposeActivities();

  void loop();

  void addBehavior(String deviceName, Behavior *behavior);
  void addFan(String name, int pinNum, int lowTemp, int highTemp, String event);
  void addLight(int pin, String name);
  void addSwitch(int pinNum, String activity);

};

#endif /* iot_h */
