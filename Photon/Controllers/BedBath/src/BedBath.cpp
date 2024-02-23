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

#define CONTROLLER_NAME "BedBath"
#define MQTT_BROKER "192.168.0.33"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

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

  // LEDs
  Device::add(new Light(D7, "BlueLED", "Bathroom", 0));
  Device::add(new Light(D7, "TestLED1", "Bathroom", 0));
  Device::add(new Light(D7, "TestLED2", "Bathroom", 0));
  Device::add(new Light(D7, "TestLED3", "Bathroom", 0));
  Device::add(new Light(D7, "TestLED4", "Bathroom", 0));

}

void loop() {
  IoT::loop();
}
