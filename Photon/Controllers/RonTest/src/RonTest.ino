/*
 * RonTest Controller
 *
 * Description: This sketch provides the bridge, PartOfDay, and Activities
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 */

#include <IoT.h>

#define CONTROLLER_NAME "RonTest"
#define MQTT_BROKER "192.168.50.33"

// Running without cloud means Photon must be put into Listen mode to flash
#define CLOUD_ENABLED false

//While debugging, use serial and disable MQTT logging if needed
SerialLogHandler logHandler;
#define MQTT_LOGGING false

//Using Threads may cause problems with other libraries, etc.
//So not doing it anymore
//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

//Used to determine I2C addresses for debugging I2C boards
//unsigned long lastScan = 0;
//unsigned long scanInterval = 15000;

int sleeping = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);   // or ANT_EXTERNAL
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CLOUD_ENABLED, MQTT_LOGGING);
    Device::add(new Device("sleeping", "All"));
}

void loop() {
    IoT::loop();
    handleSleeping();
//    scanI2Caddresses();
}

void handleSleeping() {

    int sleepingChanged = Device::getChangedValue("sleeping");
    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);

        sleeping = sleepingChanged;
    }
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
