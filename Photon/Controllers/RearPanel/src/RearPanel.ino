/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle
 
 This is the bridge controller, so all activities should be added here.
 This controller will then repeat them to MQTT.
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define ADDRESS 1   // PWM board address A0 jumper set

void setup() {
    IoT::begin("192.168.10.184", "RearPanel");

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

    // Activities/States
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));
    Device::add(new Device("cleaning"));
}

void loop() {

    IoT::loop();
    
    int sleeping = IoT::getChangedValue("sleeping");
    int partOfDay = IoT::getChangedValue("partofday");
    int cleaning = IoT::getChangedValue("cleaning");

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
            setAllInsideLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllInsideLights( 0 );
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
    Device::setValue("piano", 50);
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
