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
 
 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
 manually connect, but everything is automatic
 after that. This allows running loop and MQTT
 even if no internet available

 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define FRONT_DOOR_TIMEOUT 5*60*1000

#define ADDRESS 1      // PWM board switches low switch on


SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

//TODO: convert to IPAddress
byte hueServer[4] = { 192, 168, 50, 21 };

IPAddress myAddress(192,168,50,17);
IPAddress netmask(255,255,255,0);
IPAddress gateway(192,168,50,1);
IPAddress dns(192,168,50,1);

IPAddress mqttAddress(192, 168, 50, 33);

bool frontDoor = false;
bool frontDoorCountdown = false;
long lastFrontDoor = 0;

int watching = 0;
int cleaning = 0;
int partOfDay = 0;
int sleeping = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    setWifiStaticIP();
    IoT::begin("192.168.50.33", "FrontPanel");
    createDevices();
}

void setWifiStaticIP() {
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    WiFi.useStaticIP();
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

void loop() {
    
    // When IoT loop() is called, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();
    
//    int blueledChanged   = Device::getChangedValue("blueled");
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

//    if( blueledChanged != -1 ) {
//        handleBlueledChange(blueledChanged);
//    }

    if( watchingChanged != -1 ) {
        handleWatchingChange(watchingChanged);
    }
    
    //TODO: Convert to resallable light switches
    handleLightSwitches();
}

//TODO: Remove after converting to resellable switches
void handleLightSwitches() {
    handleLightSwitch("Ceiling");
    handleLightSwitch("KitchenCeiling");
    handleLightSwitch("Sink");
    handleLightSwitch("Cabinets");
    handleLightSwitch("RightTrim");
    handleLightSwitch("LeftTrim");
    handleLightSwitch("DoorSide");
    handleLightSwitch("OtherSide");
    handleLightSwitch("FrontPorch");
    handleLightSwitch("FrontAwning");
}

void handleLightSwitch(String name) {
    int lightSwitch = Device::getChangedValue(name+"Switch");
    if( lightSwitch == -1) return;
    Log.info("handleLightSwitch hasChanged: %d",lightSwitch);
    Device *device = Device::get(name);
    if( lightSwitch > 0 ) {
        device->setValue(100);
    } else {
        device->setValue(0);
    }
}

void handleSleepingChange(int sleeping) {
    Log.info("sleeping has changed: %d",sleeping);

    int partOfDay = Device::value("PartOfDay");
    
    // Alexa, Good morning
//    Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleeping,partOfDay);
    if( sleeping == AWAKE) { //} && partOfDay > SUNSET ) {
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

//void handleBlueledChange(int blueled) {
//    if( blueled > 0 ) {
//        Log.info("blueled did turn on");
//        setBlueledLights(100);
//    } else {
//        Log.info("blueled did turn off");
//        setBlueledLights(0);
//    }
//}

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

//void setBlueledLights(int value) {  // 0 = Off, else On
//    Log.info("setBlueledLights %d", value);
//
//    if( value > 0 ) {   // Turn on TV lights
//        Device::setValue("Sink", 50);
//
//    } else {    // Turn off TV lights
//        Device::setValue("Sink", 0);
//    }
//}

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
