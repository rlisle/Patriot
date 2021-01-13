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
#include <PatriotSwitch.h>

void setup() {
    IoT::begin("192.168.10.184","LeftSlide");

    // Lights
    Device::add(new Light(TX, "Couch"));
    Device::add(new Light(RX, "LeftVertical"));
    
    // Switches
    Device::add(new Switch(A0, "CouchSwitch"));
    Device::add(new Switch(A1, "LeftVerticalSwitch"));
    
    // Activities/States
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));
    Device::add(new Device("cleaning"));
}

void loop() {

    IoT::loop();

    int sleepingChanged = Device::getChangedValue("sleeping");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int cleaningChanged = Device::getChangedValue("cleaning");

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
