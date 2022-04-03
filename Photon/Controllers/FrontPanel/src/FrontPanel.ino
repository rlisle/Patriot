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

#define FRONT_DOOR_TIMEOUT 5*60*1000

#define ADDRESS 1      // PWM board switches low switch on

//IoT *iot;

bool frontDoor = false;
bool frontDoorCountdown = false;
long lastFrontDoor = 0;

int watching = 0;
int cleaning = 0;
int partOfDay = 0;
int sleeping = 0;

void setup() {
    IoT::begin("192.168.50.33", "FrontPanel");
    createDevices();
    handleDaylightSavings();
}

void createDevices() {

    // Inside Lights
    Device::add(new NCD8Light(ADDRESS, 1, "KitchenCeiling", "Kitchen", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "Sink", "Kitchen", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RightTrim", "Kitchen", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "LeftTrim", "Living Room", 2));
    Device::add(new Light(D2, "Ceiling", "Kitchen", 2));
    Device::add(new Light(D3, "Cabinets", "Kitchen", 2));

    // Outside Lights
    Device::add(new NCD8Light(ADDRESS, 0, "DoorSide", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "OtherSide", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "FrontAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 7, "FrontPorch", "Outside", 2));

    // Switches
    Device::add(new Switch(A5, "CabinetsSwitch", "Kitchen"));
    Device::add(new Switch(A6, "RightTrimSwitch", "Kitchen"));
    Device::add(new Switch(A7, "SinkSwitch", "Kitchen"));
    Device::add(new Switch(RX, "KitchenCeilingSwitch", "Kitchen"));
    Device::add(new Switch(TX, "LeftTrimSwitch", "Living Room"));
    Device::add(new Switch(A1, "CeilingSwitch", "Living Room"));
    Device::add(new Switch(A2, "DoorSideSwitch", "Outside"));
    Device::add(new Switch(A3, "OtherSideSwitch", "Outside"));
    Device::add(new Switch(A4, "FrontPorchSwitch", "Outside"));
    Device::add(new Switch(A0, "FrontAwningSwitch", "Outside"));

    // Other devices we monitor
    // TODO: could we define these automatically when getChangedValue is called?
    Device::add(new Device("blueled", "Test"));
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
    Device::add(new Device("watching", "All"));
}

// TODO: refactor to IoT
// from 2nd Sunday of March through 1st Sunday of November
// 2022 3/13 - 11/6, 2023 3/12 - 11/5, 2024 3/10 - 11/3
void handleDaylightSavings() {
    int month = Time.month();
    if(month > 3 && month < 11) {
        Time.beginDST();
    } else if(month == 3) {
        handleDSTMarch();
    } else if(month == 11) {
        handleDSTNovember();
    }
}

void handleDSTMarch() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day <= 7) return;
    
    switch(day) {
        case 1:     // Sunday
            if(day == 8 && hour < 2) return;
            break;
        case 2:
            if(day < 9) return;
        case 3:
            if(day < 10) return;
        case 4:
            if(day < 11) return;
        case 5:
            if(day < 12) return;
        case 6:
            if(day < 13) return;
        case 7:     // Saturday
        default:
            if(day < 14) return;
    }
    Time.beginDST();
}

void handleDSTNovember() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day > 7) return;
    
    switch(day) {
        case 1:     // Sunday
            if(day == 1 && hour >= 2) return;
            break;
        case 2:
            if(day > 2) return;
            break;
        case 3:
            if(day > 3) return;
            break;
        case 4:
            if(day > 4) return;
            break;
        case 5:
            if(day > 5) return;
            break;
        case 6:
            if(day > 6) return;
            break;
        case 7:     // Saturday
        default:
            if(day > 7) return;
    }
    Time.beginDST();
}


void loop() {
    
    // When IoT loop() is called, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();
    
    int blueledChanged   = Device::getChangedValue("blueled");
    int cleaningChanged  = Device::getChangedValue("cleaning");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int sleepingChanged  = Device::getChangedValue("sleeping");
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
    
    //TODO: Convert to resallable light switches
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
        setAllInsideLights( 100 );
    } else {
        Log.info("cleaning did turn off");
        setAllInsideLights( 0 );
    }
}

void handleWatchingChange(int watching) {
    if( watching > 0 ) {
        Log.info("watching did turn on");
        setWatchingLights( 100 );
    } else {
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
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllOutsideLights(0);
    setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("Sink", 60);
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
        Device::setValue("Sink", 50);
        
    } else {    // Turn off TV lights
        Device::setValue("Sink", 0);
    }
}

// "Alexa, watch tv"
void setWatchingLights(int value) {
    Log.info("setWatchingLights %d", value);

    // TODO: What if watching TV while also washing dishes, etc?
    if( value > 0 ) {   // Turn on "watching TV" lights
        Device::setValue("Sink", 50);
        Device::setValue("RightTrim", 0);
        Device::setValue("LeftTrim", 100);
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
