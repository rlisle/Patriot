/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
 
 This isn't the bridge controller, so we shouldn't receive any activities from particle.io
 
  Switch wiring
  Top left:       RX  tape "Cabinet"
  Top right:     TX
  2nd left:       A7  tape "Sink"
  2nd middle: A5  tape "Ceiling"
  2nd right:     A6  tape "Indirect"
  3rd left:        A1  "Ceiling"
  3rd l-m:        A2  "DS Flood Lights"
  3rd r-m:        A3  "ODS Flood Lights"
  3rd right:      A4  "Porch Lights"
  bottom:        A0  "Light"

  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    2. "particle flash FrontPanel"
 
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define ADDRESS 1      // PWM board switches low switch on

IoT *iot;

              
void setup() {
    IoT::begin("192.168.10.184", "FrontPanel");

    Device::add(new NCD8Light(ADDRESS, 0, "DoorSide", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "KitchenCeiling", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "Sink", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "OtherSide", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RightTrim",2));
    Device::add(new NCD8Light(ADDRESS, 5, "LeftTrim",2));
    Device::add(new NCD8Light(ADDRESS, 6, "FrontAwning", 2));
    Device::add(new NCD8Light(ADDRESS, 7, "FrontPorch", 2));

    Device::add(new Light(D2, "Ceiling", 2));
    Device::add(new Light(D3, "Cabinets", 2));

    Device::add(new Switch(A5, "CabinetsSwitch"));
    Device::add(new Switch(A6, "RightTrimSwitch"));
    Device::add(new Switch(A7, "SinkSwitch"));
    Device::add(new Switch(RX, "KitchenCeilingSwitch"));
    Device::add(new Switch(TX, "LeftTrimSwitch"));
    Device::add(new Switch(A1, "CeilingSwitch"));
    Device::add(new Switch(A2, "DoorSideSwitch"));
    Device::add(new Switch(A3, "OtherSideSwitch"));
    Device::add(new Switch(A4, "FrontPorchSwitch"));
    Device::add(new Switch(A0, "FrontAwningSwitch"));
    
    // Activities/States
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));
    Device::add(new Device("cleaning"));
    Device::add(new Device("cooking"));
}

void loop() {
    
    // When IoT loop() is call, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();
    
    int sleeping  = Device::getChangedValue("sleeping");
    int partOfDay = Device::getChangedValue("partofday");
    int cleaning  = Device::getChangedValue("cleaning");
    int cooking   = Device::getChangedValue("cooking");

    if( sleeping != -1 ) {
        handleSleepingChange(sleeping, partOfDay);
    }

    if( partOfDay != -1 ) {
        handlePartOfDayChange(partOfDay);
    }
    
    if( cooking != -1 ) {
        handleCookingChange(cooking);
    }

    if( cleaning != -1 ) {
        handleCleaningChange(cleaning);
    }

    handleLightSwitches();
}

void handleLightSwitches() {
    IoT::handleLightSwitch("Ceiling");
    IoT::handleLightSwitch("KitchenCeiling");
    IoT::handleLightSwitch("Sink");
    IoT::handleLightSwitch("Cabinets");
    IoT::handleLightSwitch("RightTrim");
    IoT::handleLightSwitch("LeftTrim");
    IoT::handleLightSwitch("DoorSide");
    IoT::handleLightSwitch("OtherSide");
    IoT::handleLightSwitch("FrontPorch");
    IoT::handleLightSwitch("FrontAwning");
}

void handleSleepingChange(int sleeping, int partOfDay) {
    Log.info("sleeping has changed: %d",sleeping);

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

void handlePartOfDayChange(int partOfDay) {

    Log.info("PartOfDay has changed: %d", partOfDay);

    if( partOfDay == SUNRISE ) {
        setSunriseLights();
    }

    if( partOfDay == DUSK ) {
        setEveningLights();
    }
}

void handleCookingChange(int cooking) {
    if( cooking > 0 ) {
        Log.info("cooking did turn on");
        setCookingLights(100);
    } else {
        //TODO: check if evening lights s/b on, etc.
        Log.info("cooking did turn off");
        setCookingLights(0);
    }
}

void handleCleaningChange(int cleaning) {
    if( cleaning > 0 ) {
        Log.info("cleaning did turn on");
        //TODO: save/restore previous states
        setAllInsideLights( 100 );
    } else {
        //TODO: save/restore previous states
        Log.info("cleaning did turn off");
        setAllInsideLights( 0 );
    }
}

void setAllActivities(int value) {
    Device::setValue("cooking", value);
    Device::setValue("cleaning", value);
}


void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("Cabinets", 50);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllOutsideLights(0);
    setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("KitchenCeiling", 50);
    Device::setValue("Sink", 50);
    Device::setValue("RightTrim", 0);
    Device::setValue("LeftTrim", 100);
    Device::setValue("Ceiling", 50);
    Device::setValue("Cabinets", 50);
    
    setAllOutsideLights(100);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    Device::setValue("KitchenCeiling", 50);
    Device::setValue("Sink", 0);
    Device::setValue("RightTrim", 0);
    Device::setValue("LeftTrim", 0);
    Device::setValue("Ceiling", 50);
    Device::setValue("Cabinets", 0);
    setAllOutsideLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
}

void setCookingLights(int value) {
    Log.info("setCookingLights %d", value);
    //TODO: save/restore previous states
    Device::setValue("KitchenCeiling", value);
    Device::setValue("Sink", value);
    Device::setValue("Cabinets", value);
}

void setAllInsideLights(int level) {
    Log.info("setAllInsideLights");
    Device::setValue("KitchenCeiling", level);
    Device::setValue("Sink", level);
    Device::setValue("RightTrim", level);
    Device::setValue("LeftTrim", level);
    Device::setValue("Ceiling", level);
    Device::setValue("Cabinets", level);
}

void setAllOutsideLights(int level) {
    Log.info("setAllOutsideLights");
    Device::setValue("DoorSide", level);
    Device::setValue("OtherSide", level);
    Device::setValue("FrontAwning", level);
    Device::setValue("FrontPorch", level);
}
