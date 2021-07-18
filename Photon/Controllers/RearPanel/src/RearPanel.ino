/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 It includes the PartOfDay plugin.
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotPartOfDay.h>
#include <PatriotCurtain.h>
#include <PatriotNCD4Switch.h>

#define ADDRESS 1      // PWM board address A0 jumper set
#define I2CR4IO4 0x20  // 4xRelay+4GPIO address

void setup() {
    IoT::begin("192.168.50.33", "RearPanel");

    // PartOfDay
    Device::add(new PartOfDay());

    Device::add(new Curtain(I2CR4IO4, 0, "Curtain"));
    Device::add(new NCD4Switch(I2CR4IO4, 0, "OfficeDoor"));

    // Inside Lights
    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "Loft", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "Piano", 2));
    // Fading OfficeTrim results in door toggling, probably due to parallel wiring
    Device::add(new NCD8Light(ADDRESS, 7, "OfficeTrim", 0));

    // Outside Lights
    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", 2));

    // Switches
    Device::add(new Switch(A0, "OfficeCeilingSwitch"));
    Device::add(new Switch(A1, "LoftSwitch"));
    Device::add(new Switch(A2, "RampPorchSwitch"));
    Device::add(new Switch(A3, "RampAwningSwitch"));
    Device::add(new Switch(A4, "RearPorchSwitch"));
    Device::add(new Switch(A5, "RearAwningSwitch"));
    // More available inputs A6, A7, TX, RX - use for door switch, motion detector, etc.

    // Activities/States - define for every other state
    // Be careful to only define in 1 (this) controller
    Device::add(new Device("sleeping"));
    Device::add(new Device("cleaning"));
    Device::add(new Device("watching"));    // TODO: move to front controller?
}

void loop() {

    IoT::loop();
    
    int sleepingChanged = Device::getChangedValue("sleeping");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int cleaningChanged = Device::getChangedValue("cleaning");
    int watchingChanged = Device::getChangedValue("watching");
    int officeDoorChanged = Device::getChangedValue("OfficeDoor");
    int partOfDay = Device::value("PartOfDay");

    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);

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

    if( officeDoorChanged != -1) {
        if( officeDoorChanged > 0 ) {   // Door opened
            if( partOfDay > SUNSET ) {
                Device::setValue("RearPorch", 100);
            }
            //TODO: chime?
        } else {                        // Door closed
            // Nothing to do when door closes
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
    Device::setValue("officeceiling",70);
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
    setAllInsideLights(0);
    setAllOutsideLights(0);
    Device::setValue("Curtain",0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
    Device::setValue("Curtain",0);
}

void setWatchingLights(int level) {
    Log.info("setWatchingLights %d", level);
    // Nothing to do
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("OfficeCeiling", value);
    Device::setValue("Loft", value);
    Device::setValue("Piano", value);
    Device::setValue("OfficeTrim", value);
}

void setAllOutsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("RampPorch", value);
    Device::setValue("RampAwning", value);
    Device::setValue("RearPorch", value);
    Device::setValue("RearAwning", value);
}
