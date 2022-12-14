/**
Voltage Example

This example supports monitoring the voltage on pin A0

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotVoltage.h>

#define CONTROLLER_NAME "VoltageTest"
#define MQTT_BROKER "192.168.50.33" // Set to your MQTT broker IP
#define CLOUD_ENABLED true          // Disable to prevent connection to Particle.io
#define MQTT_LOGGING true

void setup() {
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CLOUD_ENABLED, MQTT_LOGGING);
    Log.info(CONTROLLER_NAME + " started.");
    Device::add(new Voltage(A0, "voltMonitor", "office"));
}

void loop() {
    IoT::loop();

    int voltageChanged = Device::getChangedValue("voltMonitor");
    if(voltageChanged != -1){
        handleVoltageChanged(voltageChanged);
    }
}

void handleVoltageChanged(int volts) {
    // Do whatever is needed based on new volts value
    if(volts >= 12) {
        Log.info("Voltage is over 12: " + String(volts));
    } else {
        Log.info("Voltage is under 12: " + String(volts));
    }
}
