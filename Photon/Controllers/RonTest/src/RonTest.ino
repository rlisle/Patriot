/*
 * RonTest Controller
 *
 * Description: This sketch is used for experimenting and testing
 * Currently testing Awning and Power
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 * - Various based on test/experimentation needs
 */

#include <IoT.h>
#include <PatriotCurtain.h>
#include <PatriotPower.h>
#include <PatriotAwning.h>

#define CONTROLLER_NAME "RonTest"
#define MQTT_BROKER "192.168.50.33"

//While debugging, use serial and disable MQTT logging if needed
SerialLogHandler logHandler;
#define MQTT_LOGGING false

#define I2CR4IO4 0x20  // 4xRelay+4GPIO address

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

int amps = 0;
int ampsThreshold = 10;     // Report changes > 10 amps

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);   // or ANT_EXTERNAL
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(I2CR4IO4, 0, "TestCurtain", "Office"));     // 2x Relays: 0, 1
    Device::add(new Awning(I2CR4IO4, 2, "TestAwning", "Outside"));     // 2x Relays: 2, 3

    // 4 GPIO
//    Device::add(new NCD4Switch(I2CR4IO4, 0, "TestDoor", "Office"));

    Device::add(new Power("amps", "Status"));
    
    Log.info("RonTest started");
}

void loop() {
    IoT::loop();
    
//    scanI2Caddresses();
    handleAmps();

}

void handleAmps() {
    int ampsChanged = Device::getChangedValue("amps");
    if( ampsChanged != -1 ) {
        if(ampsChanged >= amps + 10) {
            Log.info("amps increased to %d",ampsChanged);
        }
        if(ampsChanged <= amps - 10) {
            Log.info("amps decreased to %d",ampsChanged);
        }
        amps = ampsChanged;
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
