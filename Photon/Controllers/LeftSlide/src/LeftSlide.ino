/**
Left Slide Controller
Description: This sketch controls the 2 lights and switch in the left slide.
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

String mqttServer = "192.168.1.10";

IoT *iot;


// Switch control functional sets of lights, not individual lights

void setup() {
    IoT::begin("192.168.1.10","LeftSlide");

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

    int sleeping = Device::getChangedValue("sleeping");
    int partOfDay = Device::getChangedValue("partofday");
    int cleaning = Device::getChangedValue("cleaning");

    if( sleeping != -1 ) {

        Log.info("sleeping has changed %d",sleeping);

        // Alexa, Good morning
        if( sleeping == AWAKE && partOfDay > SUNSET ) {
            setMorningLights();
        }

        // Alexa, Bedtime
        if( sleeping == RETIRING ) {
            setBedtimeLights();
        }

        // Alexa, Goodnight
        if( sleeping == ASLEEP ) {
            setSleepingLights();
        }
    }

    if( partOfDay != -1 ) {

        Log.info("partOfDay has changed: %d", partOfDay);

        if( partOfDay == SUNRISE ) {
            // Turn off lights at sunrise
            setSunriseLights();
        }

        if( partOfDay == DUSK ) {
            // Turn on lights after sunset
            setEveningLights();
        }
    }

    if( cleaning != -1 ) {
        if( cleaning > 0 ) {
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
