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

    // Inside Lights
    Device::add(new NCD8Light(ADDRESS, 1, "KitchenCeiling", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "Sink", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RightTrim",2));
    Device::add(new NCD8Light(ADDRESS, 5, "LeftTrim",2));
    Device::add(new Light(D2, "Ceiling", 2));
    Device::add(new Light(D3, "Cabinets", 2));

    // Outside Lights
    Device::add(new NCD8Light(ADDRESS, 0, "DoorSide", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "OtherSide", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "FrontAwning", 2));
    Device::add(new NCD8Light(ADDRESS, 7, "FrontPorch", 2));

    // Switches
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

    // Other devices we monitor
    // TODO: could we define these automatically when getChangedValue is called?
    Device::add(new Device("sleeping"));
    Device::add(new Device("partofday"));
    Device::add(new Device("cleaning"));
    Device::add(new Device("blueled"));
    Device::add(new Device("watching"));
}

void loop() {
    
    // When IoT loop() is called, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();
    
    int sleepingChanged  = Device::getChangedValue("sleeping");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int cleaningChanged  = Device::getChangedValue("cleaning");
    int blueledChanged   = Device::getChangedValue("blueled");
    int watchingChanged  = Device::getChangedValue("watching");

    if( sleepingChanged != -1 ) {
        handleSleepingChange(sleepingChanged);
    }

    if( partOfDayChanged != -1 ) {
        handlePartOfDayChange(partOfDayChanged);
    }
    
    if( cleaningChanged != -1 ) {
        handleCleaningChange(cleaningChanged);
    }

    if( blueledChanged != -1 ) {
        handleBlueledChange(blueledChanged);
    }

    if( watchingChanged != -1 ) {
        handleWatchingChange(watchingChanged);
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

void handleSleepingChange(int sleeping) {
    Log.info("sleeping has changed: %d",sleeping);

    int partOfDay = Device::value("PartOfDay");
    
    // Alexa, Good morning
    Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleeping,partOfDay);
    if( sleeping == AWAKE && partOfDay > SUNSET ) {
        Log.info("It is good morning");
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

void handleBlueledChange(int blueled) {
    if( blueled > 0 ) {
        Log.info("blueled did turn on");
        setBlueledLights(100);
    } else {
        Log.info("blueled did turn off");
        setBlueledLights(0);
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

void handleWatchingChange(int watching) {
    if( watching > 0 ) {
        Log.info("watching did turn on");
        //TODO: save/restore previous states
        setWatchingLights( 100 );
    } else {
        //TODO: save/restore previous states
        Log.info("watching did turn off");
        setWatchingLights( 0 );
    }
}

void setAllActivities(int value) {
    Device::setValue("blueled", value);
    Device::setValue("cleaning", value);
    Device::setValue("watching", value);
}


void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("Sink", 40);
    Device::setValue("Cabinets", 50);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllOutsideLights(0);
    setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("KitchenCeiling", 70);
    Device::setValue("Sink", 60);
    Device::setValue("RightTrim", 0);
    Device::setValue("LeftTrim", 100);
    Device::setValue("Ceiling", 70);
    Device::setValue("Cabinets", 60);
    
    setAllOutsideLights(100);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
}

// This is currently the livingroom "Turn on lights" Huh???
void setBlueledLights(int value) {  // 0 = Off, else On
    Log.info("setBlueledLights %d", value);
    
    if( value > 0 ) {   // Turn on TV lights
        Device::setValue("KitchenCeiling", 60);
        Device::setValue("Sink", 50);
        Device::setValue("Ceiling", 70);
        
    } else {    // Turn off TV lights
        Device::setValue("KitchenCeiling", 0);
        Device::setValue("Sink", 0);
        Device::setValue("Ceiling", 0);
    }
}

// "Alexa, watch tv"
void setWatchingLights(int value) {
    Log.info("setWatchingLights %d", value);

    // TODO: What if watching TV while also washing dishes, etc?
    if( value > 0 ) {   // Turn on "watching TV" lights
        Device::setValue("KitchenCeiling", 50);
        Device::setValue("Sink", 50);
        Device::setValue("RightTrim", 0);
        Device::setValue("LeftTrim", 100);
        Device::setValue("Ceiling", 65);
        Device::setValue("Cabinets", 0);
    }
}

void setAllInsideLights(int level) {
    Log.info("setAllInsideLights level %d", level);
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
