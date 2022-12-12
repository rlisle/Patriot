/**
Voltage Example

This example supports monitoring the voltage on pin A0

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <Voltage.h>

#define CONTROLLER_NAME "VoltageTest"
#define MQTT_BROKER "192.168.50.33" // Set to your MQTT broker IP
#define CLOUD_ENABLED true          // Disable to prevent connection to Particle.io
#define MQTT_LOGGING true

unsigned long lastVScan = 0;
unsigned long vScanInterval = 5000; // Send voltage status to MQTT every 5 seconds

void setup() {
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CLOUD_ENABLED, MQTT_LOGGING);
    Log.info(CONTROLLER_NAME + " started.");
    Device::add(new Curtain(0, 1, "curtains"));

}

void loop() {
    IoT::loop();
    
    if(millis() > lastVScan + vScanInterval){
        lastVScan = millis();
        handleVoltageMonitor();
    }
}

void handleVoltageMonitor() {
    int volts = analogRead(A0);
    IoT::publishValue("volts", volts);
}

