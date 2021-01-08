/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 It includes the PartOfDay plugin.
 
 PartOfDay Values
  0 SUNRISE
  1 MORNING
  2 NOON
  3 AFTERNOON
  4 SUNSET
  5 DUSK
  6 NIGHT
  7 DAWN

  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotPartOfDay.h>

#define ADDRESS 1   // PWM board address A0 jumper set

void setup() {
    IoT::begin("192.168.10.184", "RearPanel");

    // PartOfDay
    Device::add(new PartOfDay());

    // Lights
    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "Loft", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "Piano", 2));
    // one unused dimmer I/O

    // Switches
    Device::add(new Switch(A0, "OfficeCeilingSwitch"));
    Device::add(new Switch(A1, "LoftSwitch"));
    Device::add(new Switch(A2, "RampPorchSwitch"));
    Device::add(new Switch(A3, "RampAwningSwitch"));
    Device::add(new Switch(A4, "RearPorchSwitch"));
    Device::add(new Switch(A5, "RearAwningSwitch"));
    // More available inputs A6, A7, TX, RX - use for door switch, motion detector, etc.

    // Activities/States - define for every other state
    Device::add(new Device("sleeping"));
    Device::add(new Device("cleaning"));
    Device::add(new Device("theater"));
}

void loop() {

    IoT::loop();
    
    int sleepingChanged = Device::getChangedValue("sleeping");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int cleaningChanged = Device::getChangedValue("cleaning");
    int theaterChanged = Device::getChangedValue("theater");

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

        Log.info("partOfDay has changed: %d", partOfDayChanged);

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
            setAllInsideLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllInsideLights( 0 );
        }
    }

    if( theaterChanged != -1 ) {
        if( theaterChanged > 0 ) {
            Log.info("theater did turn on");
            setTheaterLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("theater did turn off");
            setTheaterLights( 0 );
        }
    }

    // SWITCHES
    IoT::handleLightSwitch("OfficeCeiling");
    IoT::handleLightSwitch("Loft");
    IoT::handleLightSwitch("RampPorch");
    IoT::handleLightSwitch("RampAwning");
    IoT::handleLightSwitch("RearPorch");
    IoT::handleLightSwitch("RearAwning");
}

void setAllActivities(int value) {
    Device::setValue("cooking", value);
    Device::setValue("cleaning", value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("piano", 20);
    Device::setValue("officeceiling",80);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllOutsideLights(0);
    setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("piano", 50);
    Device::setValue("officeceiling",80);
    setAllOutsideLights(100);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    Device::setValue("OfficeCeiling", 80);
    Device::setValue("Loft", 0);
    Device::setValue("piano", 50);
    setAllOutsideLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
}

void setTheaterLights(int level) {
    Log.info("setTheaterLights %d", level);
    // Nothing to do
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("OfficeCeiling", value);
    Device::setValue("Loft", value);
    Device::setValue("Piano", value);
}

void setAllOutsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("RampPorch", value);
    Device::setValue("RampAwning", value);
    Device::setValue("RearPorch", value);
    Device::setValue("RearAwning", value);
}
