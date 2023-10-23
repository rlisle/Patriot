/*
 * RonTest Controller
 *
 * Description: This sketch is used for experimenting and testing
 * Currently testing Photon2
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 * - Various based on test/experimentation needs
 */

#include "Particle.h"
#include <IoT.h>

#define CONTROLLER_NAME "RonTest"
#define MQTT_BROKER "192.168.1.66"
#define MQTT_LOGGING false // using USB

//While debugging, use serial and disable MQTT logging if needed
SerialLogHandler logHandler;

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);   // or ANT_EXTERNAL
//    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);

    // Lights
    Device::add(new Light(A2, "TestLight1", "Living Room", 2));

    // PIR
    Device::add(new PIR(A0, "TestMotion1", "Living Room", 300));

    Log.info("RonTest started");
}

void loop() {
    IoT::loop();
    
//    scanI2Caddresses();

//    Watchdog.refresh();   // This is done in IoT::loop now

}

// Diagnostic Functions
// Search all I2C addresses every 15 seconds -
// leave this here for future use (comment out if not used)
//void scanI2Caddresses() {
//    if(millis() > lastScan + scanInterval){
//        lastScan = millis();
//
//        bool devicesFound = false;
//        String newDevices = "Devices at: ";
//        //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
//        for(int i = 1; i < 128; i++){
//            //Make a general call to device at the current address to see if anything responds.
//            Wire.beginTransmission(i);
//            byte status = Wire.endTransmission();
//            if(status == 0){
//                //Device found so append it to our device list event string
//                newDevices.concat(i);
//                newDevices.concat(", ");
//                devicesFound = true;
//            }
//
//        }
//        if(devicesFound){
//            Log.info(newDevices);
//        }else{
//            Log.info("No Devices Found");
//        }
//    }
//}
