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
#include <PatriotCurtain.h>
#include <PatriotNCD4Switch.h>

void setup() {
    IoT::begin("192.168.10.184", "RonTest");

    Device::add(new Light(7, "blueLed", false, true));
    
    Device::add(new Curtain(0x20,0,"Curtain"));

    // Basic devices allow Alexa to control the name
    // and can also turn off other activities.
    Device::add(new Device("cleaning"));
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));
}

unsigned long scanInterval = 15000;
unsigned long lastScan;

void loop() {

    // Search all I2C addresses every 15 seconds - leave this here commented out for future use
//     if(millis() > lastScan + scanInterval){
//         lastScan = millis();
//
//         bool devicesFound = false;
//         String newDevices = "Devices at: ";
//         //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
//         for(int i = 1; i < 128; i++){
//             //Make a general call to device at the current address to see if anything responds.
//             Wire.beginTransmission(i);
//             byte status = Wire.endTransmission();
//             if(status == 0){
//                 //Device found so append it to our device list event string
//                 newDevices.concat(i);
//                 newDevices.concat(", ");
//                 devicesFound = true;
//             }
//
//         }
//         if(devicesFound){
//             Log.info(newDevices);
//         }else{
//             Log.info("No Devices Found");
//         }
//     }

    IoT::loop();

    int changedSleeping = Device::getChangedValue("sleeping");
    int changedPartOfDay = Device::getChangedValue("partofday");
    int sleeping = Device::value("sleeping");
    int partOfDay = Device::value("partofday");

    if( changedSleeping != -1 ) {
        
        Log.info("sleeping has changed: %d",sleeping);

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleeping,partOfDay);
        if( changedSleeping == AWAKE && partOfDay > SUNSET ) {
            Log.info("It is good morning");
            setMorningLights();
        }

        // Alexa, Bedtime
        if( changedSleeping == RETIRING ) {
            setMorningLights();
        }

        // Alexa, Goodnight
        if( changedSleeping == ASLEEP ) {
            setAllInsideLights(0);
        }
    }

    if( changedPartOfDay != -1 ) {

        Log.info("PartOfDay has changed: %d", partOfDay);

        // Turn off lights at sunrise
        if( changedPartOfDay == SUNRISE ) {
            setAllInsideLights(0);
        }

        // Turn on lights in the evening
        if( changedPartOfDay == DUSK ) {
            setEveningLights();
        }
    }
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("BlueLED", 100);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("BlueLED", 100);
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("BlueLED", value);
}
