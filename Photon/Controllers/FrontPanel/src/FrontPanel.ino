/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
  
  Switch wiring
  Top left:       RX  tape label "Cabinet" -> D3 green -> gold
  Top right:     TX no label (right trim) #4 yellow
  2nd left:       A7  tape label "Sink" #2 white
  2nd middle: A5  tape label "Ceiling" (kitchen) #1 red
  2nd right:     A6  tape label "Indirect" (left trim) #5 blue
  gnd                     brown?

  3rd left:        A1  "Ceiling" D2 blue -> silver
  3rd l-m:        A2  "DS Flood Lights" #0 green
  3rd r-m:        A3  "ODS Flood Lights" #3 yellow
  3rd right:      A4  "Porch Lights" #7 red
  bottom:        A0  "Light" (awning) #6 white
 

  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. Put Photon into listen mode using buttons
    4. "particle flash FrontPanel" or "ffp"
 
  Photon is 430033000f47343339383037 FrontPanel
  RSSI = -66dBm  on 10/1/22
 
 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
 manually connect, but everything is automatic
 after that. This allows running loop and MQTT
 even if no internet available

 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotNCD8Light.h>

#define CONTROLLER_NAME "FrontPanel"
#define MQTT_BROKER "192.168.50.33"
#define ADDRESS 1      // PWM board lowest switch on

#define VMPIN A0

// Until mystery hangs understood, leave in automatic
#define CONNECT_TO_CLOUD true
//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

int sleeping = 0;

unsigned long lastVScanTime = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CONNECT_TO_CLOUD, MQTT_LOGGING);
    createDevices();
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

    // Complex Calculation pseudo-devices
    Device::add(new Device("sleeping", "All"));
}

void loop() {

    // When IoT loop() is called, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();

    if(millis() > lastVScanTime + 5000){    // Check voltage every 5 seconds
        lastVScanTime = millis();
        handleVoltageMonitor();
    }

    //TODO: calculate sleeping state based on time, motion, and doors
//    int sleepingChanged  = Device::getChangedValue("sleeping");
//
//    if( sleepingChanged != -1 ) {
//        handleSleepingChange(sleepingChanged);
//    }
}

void handleVoltageMonitor() {
    int volts = analogRead(VMPIN);
    IoT::publishValue("frontpanel/volts", volts);
}


