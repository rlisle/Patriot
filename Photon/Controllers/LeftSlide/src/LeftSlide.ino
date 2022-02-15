/**
Left Slide Controller
Description: This sketch controls the 2 lights and switches in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash LeftSlide"
 
    I/Os selected to be all on the same side
     2 switch connections:
       A0 Watch TV
       A1 Reading
    2 LED driver boards
       TX Vertical Lights
       RX Reading lights
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotPIR.h>

void setup() {
    IoT::begin("192.168.50.33","LeftSlide");

    // Photon I/O
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room"));
    
    // Lights
    Device::add(new Light(A7, "Couch", "Living Room", 2));
    Device::add(new Light(A5, "LeftVertical", "Living Room", 2));
        
    // Activities/States
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
    Device::add(new Device("watching", "All"));
}

void loop() {

    IoT::loop();

    int sleepingChanged = Device::getChangedValue("sleeping");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int cleaningChanged = Device::getChangedValue("cleaning");
    int livingRoomMotionChanged = Device::getChangedValue("LivingRoomMotion");
    int watchingChanged = Device::getChangedValue("watching");


    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);

        int partOfDay = Device::value("PartOfDay");

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleepingChanged,partOfDay);
        if( sleepingChanged == AWAKE && partOfDay > SUNSET ) {
            Log.info("It is good morning");
            setMorningLights();
        }

        // Alexa, Bedtime
        if( sleepingChanged == RETIRING ) {
            setBedtimeLights();
        }

        // Alexa, Goodnight
        if( sleepingChanged == ASLEEP ) {
            setSleepingLights();
        }
    }

    if( partOfDayChanged != -1 ) {

        Log.info("partOfDay has changed: %d", partOfDay);

        if( partOfDayChanged == SUNRISE ) {
            // Turn off lights at sunrise
            Log.info("It is sunrise");
            setSunriseLights();
        }

        if( partOfDayChanged == DUSK ) {
            // Turn on lights after sunset
            Log.info("It is dusk");
            setEveningLights();
        }
        
        if( watchingChanged != -1 ) {
            if( watchingChanged > 0 ) {
                Log.info("watching did turn on");
                setWatchingLights( 100 );
            } else {
                //TODO: check if evening lights s/b on, etc.
                Log.info("watching did turn off");
                setWatchingLights( 0 );
            }
        }
        
        if( livingRoomMotionChanged != -1) {
            // Just for testing - turn off piano when motion stops
            if( livingRoomMotionChanged > 0 ) {   // Motion detected
    //            if( partOfDay > SUNSET ) {
                    Device::setValue("LeftVertical", 50);
    //                Device::setValue("OfficeTrim", 100);
    //            }
                //TODO: chime?
            } else {                        // Door closed
                // Nothing to do when motion stops
                Device::setValue("LeftVertical", 0);
    //            Device::setValue("OfficeTrim", 0);
            }
        }

    }

    if( cleaningChanged != -1 ) {
        if( cleaningChanged > 0 ) {
            Log.info("cleaning did turn on");
            setAllLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllLights( 0 );
        }
    }

    // SWITCHES
    IoT::handleLightSwitch("Couch");
    IoT::handleLightSwitch("LeftVertical");
}

void setAllActivities(int value) {
    Device::setValue("cleaning", value);
    Device::setValue("watching", value);
}

void setAllLights(int value) {
    Log.info("setAllLights %d",value);
    Device::setValue("Couch", value);
    Device::setValue("LeftVertical", value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    setAllLights(30);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    setAllLights(50);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    setAllLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllLights(0);
}

void setWatchingLights(int level) {
    Log.info("setWatchingLights %d", level);
    Device::setValue("Couch", 30)
}

