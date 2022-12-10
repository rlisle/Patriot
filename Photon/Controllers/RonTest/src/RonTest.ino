/*
 * RonTest Controller
 *
 * Description: This sketch is used for experimenting and testing
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 * - Various based on test/experimentation needs
 */

#include <IoT.h>
//#include <PatriotCurtain.h>
//#include <PatriotNCD4Switch.h>
//#include <PatriotNCD4Relay.h>
#include <PatriotNCD8Switch.h>

#define CONTROLLER_NAME "RonTest"
#define MQTT_BROKER "192.168.50.33"

// Running without cloud means Photon must be put into Listen mode to flash
#define CLOUD_ENABLED true

//While debugging, use serial and disable MQTT logging if needed
SerialLogHandler logHandler;
#define MQTT_LOGGING true

#define ADDRESS 1      // PWM board address A0 jumper set
#define I2CR4IO4 0x20  // 4xRelay+4GPIO address
#define I2CDIO8 0x20   // 8xGPIO address (no jumpers)

//Using Threads may cause problems with other libraries, etc.
//So not doing it anymore
//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_MODE(AUTOMATIC);

//Used to determine I2C addresses for debugging I2C boards
unsigned long lastScan = 0;
unsigned long scanInterval = 15000;

int sleeping = 0;
int switch1 = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);   // or ANT_EXTERNAL
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CLOUD_ENABLED, MQTT_LOGGING);
    
    Device::add(new Device("sleeping", "All"));

    // I2CDIO8 - 8 GPIO I2C board $33
    Device::add(new NCD8Switch(I2CDIO8, 0, "Switch1", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 1, "Switch2", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 2, "Switch3", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 3, "Switch4", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 4, "Switch5", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 5, "Switch6", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 6, "Switch7", "Office" ));
    Device::add(new NCD8Switch(I2CDIO8, 7, "Switch8", "Office" ));

    // I2CIO4R4G5LE board
    // 4 Relays
//    Device::add(new Curtain(I2CR4IO4, 0, "TestCurtain", "Office"));     // 2x Relays: 0, 1
    
    // 4 GPIO
//    Device::add(new NCD4Switch(I2CR4IO4, 0, "TestDoor", "Office"));

    Log.info("RonTest started");
}

void loop() {
    IoT::loop();
    handleSleeping();
    handleSwitch1();
//    scanI2Caddresses();
}

void handleSleeping() {

    int sleepingChanged = Device::getChangedValue("sleeping");
    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);

        sleeping = sleepingChanged;
    }
}

void handleSwitch1() {

    int switch1Changed = Device::getChangedValue("switch1");
    if( switch1Changed != -1 ) {

        Log.info("Switch1 has changed %d",switch1Changed);

        switch1 = switch1Changed;
    }
}


// Diagnostic Functions
// Search all I2C addresses every 15 seconds -
// leave this here for future use (comment out if not used)
void scanI2Caddresses() {
    if(millis() > lastScan + scanInterval){
        lastScan = millis();
        
        bool devicesFound = false;
        String newDevices = "Devices at: ";
        //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
        for(int i = 1; i < 128; i++){
            //Make a general call to device at the current address to see if anything responds.
            Wire.beginTransmission(i);
            byte status = Wire.endTransmission();
            if(status == 0){
                //Device found so append it to our device list event string
                newDevices.concat(i);
                newDevices.concat(", ");
                devicesFound = true;
            }
            
        }
        if(devicesFound){
            Log.info(newDevices);
        }else{
            Log.info("No Devices Found");
        }
    }
}
