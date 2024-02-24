/* 
 * Project BedBath
 * Author: Ron Lisle
 * Date: 1/14/24
 * 
 * Currently using to debug Patriot with USB
 * 'particle serial monitor --follow' or shortcut 'debugusb'
 * 
 */
#include "IoT.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// State
int testDoor = -1;
bool isTimingTestDoor = false;

// Behaviors
#include "Settings.h"
#include "Behaviors.h"
#include "EventHandlers.h"

void setup() {
//  WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
//  WiFi.selectAntenna(ANT_INTERNAL);
//  WiFi.useDynamicIP();

  IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);

  // Behaviors
  setNextMinuteHandler(handleNextMinute);
  Device::setAnyChangedHandler(updateLights);

  Device::add(new Device("AnyoneHome", "Status", 'S'));
  Device::add(new Device("RonHome", "Status", 'S'));
  Device::add(new Device("ShelleyHome", "Status", 'S'));
  Device::add(new Device("Nighttime", "Status", 'S'));
  Device::add(new Device("Cleaning", "Status", 'S'));
  Device::add(new Device("Sleeping", "Status", 'S'));
  Device::add(new Device("Bathroom", "Status", 'S'));

  // LEDs
  Device::add(new Light(D13, "TestLED1", "Bathroom", 2));
  Device::add(new Light(D14, "TestLED2", "Bathroom", 2));
  Device::add(new Light(D15, "TestLED3", "Bathroom", 1));
  Device::add(new Light(D16, "TestLED4", "Bathroom", 1));

  // Inputs
  Device::add(new Device("TestDoor", "Status", 'S', handleTestDoor));

}

void loop() {
  IoT::loop();
}
