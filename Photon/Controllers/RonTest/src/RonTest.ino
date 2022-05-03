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
#include <PatriotLight.h>

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

//TODO: convert to IPAddress
byte hueServer[4] = { 192, 168, 50, 21 };

IPAddress myAddress(192,168,50,40);
IPAddress netmask(255,255,255,0);
IPAddress gateway(192,168,50,1);
IPAddress dns(192,168,50,1);

IPAddress mqttAddress(192, 168, 50, 33);

unsigned long lastScan = 0;
unsigned long scanInterval = 15000;

int blueLED = 0;
int partOfDay = 0;
int sleeping = 0;

void setup() {
//    WiFi.selectAntenna(ANT_EXTERNAL); //TODO: connect external antenna
    setWifiStaticIP();
    IoT::begin("192.168.50.33", "RonTest");
    createDevices();
}

void setWifiStaticIP() {
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    WiFi.useStaticIP();
}

void createDevices() {
    Device::add(new Light(7, "blueLed", "Office", false, true));
    
    // Basic devices allow Alexa to control the name
    // and can also turn off other activities.
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
}

void loop() {

    IoT::loop();
    
    handlePartOfDay();  // Do this and sleeping first
    handleSleeping();
    
//    scanI2Caddresses();

}

void handlePartOfDay() {
    
    int partOfDayChanged = Device::getChangedValue("partofday");
    if( partOfDayChanged != -1 ) {

        Log.info("partOfDay has changed: %d", partOfDayChanged);

        if( partOfDayChanged == SUNRISE ) {
            Log.info("It is sunrise");
            setSunriseLights();
        }

        if( partOfDayChanged == DUSK ) {
            Log.info("It is dusk");
            setEveningLights();
        }
        partOfDay = partOfDayChanged;
    }
}

void handleSleeping() {

    int sleepingChanged = Device::getChangedValue("sleeping");
    if( sleepingChanged != -1 ) {
        
        Log.info("sleeping has changed %d",sleepingChanged);

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleepingChanged,partOfDay);
        if( sleepingChanged == AWAKE) {
            Log.info("It is AWAKE");
            if(partOfDay > SUNSET || (partOfDay==0 && Time.hour() < 8)) {
                Log.info("It is morning");
                setMorningLights();
            }
        }

        // Alexa, Bedtime
        if( sleepingChanged == RETIRING ) {
            setBedtimeLights();
        }

        // Alexa, Goodnight
        if( sleepingChanged == ASLEEP ) {
            setSleepingLights();
        }
        
        sleeping = sleepingChanged;
    }
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    Device::setValue("BlueLED", 0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("BlueLED", 100);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("BlueLED", 100);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    Device::setValue("BlueLED", 100);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    Device::setValue("BlueLED", 0);
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
