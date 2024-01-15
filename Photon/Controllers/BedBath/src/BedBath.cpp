/* 
 * Project BedBath
 * Author: Ron Lisle
 * Date: 1/14/24
 */
#include "Particle.h"
#include "IoT.h"
#include "secrets.h"

#define CONTROLLER_NAME "BedBath"
#define MQTT_BROKER "192.168.0.33"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Generally only use 1 or the other
// Write logs to MQTT if true
#define MQTT_LOGGING false
// or show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

void setup() {
//  WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
  WiFi.selectAntenna(ANT_INTERNAL);
  //WiFi.useDynamicIP();

  IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
}

void loop() {
  IoT::loop();
}
