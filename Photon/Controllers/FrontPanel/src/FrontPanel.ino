/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
  
  Converting to Photon 2 - "front_panel2"
 
  Switch wiring
  Top left:       tape label "Cabinet" -> D3 green -> gold
  Top right:     no label (right trim) #4 yellow
  2nd left:       tape label "Sink" #2 white
  2nd middle: tape label "Ceiling" (kitchen) #1 red
  2nd right:     tape label "Indirect" (left trim) #5 blue
  gnd              brown?

  3rd left:        "Ceiling" D2 blue -> silver
  3rd l-m:        "DS Flood Lights" #0 green
  3rd r-m:        "ODS Flood Lights" #3 yellow
  3rd right:      "Porch Lights" #7 red
  bottom:        "Light" (awning) #6 white
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. Put Photon into listen mode using buttons
    4. "particle flash front_panel2" or "ffp" (was FrontPanel)
 
  Photon 2 is ??? front_panel2
 
  Photon was 430033000f47343339383037 FrontPanel
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
#include <PatriotNCD8Switch.h>
#include <PatriotVoltage.h>
//#include <PatriotPower.h>

#define CONTROLLER_NAME "FrontPanel"
#define MQTT_BROKER "192.168.50.33"
#define ADDRESS 1      // PWM board lowest switch on

#define VMPIN A0

// Until mystery hangs understood, leave in automatic
#define CONNECT_TO_CLOUD true
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

int sleeping = 0;
int voltage = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    createDevices();
}

void createDevices() {

    //TODO: Replace all lights with NCD16Lights
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

    //TODO: I/O pins not available. Will need a separate NCD board
    // 12V Monitor (actually 14.27) with 10:1 R-Ladder
    // Adjust fullScale to reflect actual R-Ladder (36.9)
//    Device::add(new Voltage(A0, "FP volts", "LivingRoom", 36.9, 10));
    
//    Device::add(new Power("Power", "Status"));

    // Complex Calculation pseudo-devices
    Device::add(new Device("sleeping", "All"));
}

void loop() {

    // When IoT loop() is called, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    IoT::loop();

//    int voltageChanged = Device::getChangedValue("FrontPanelVolts");
//    if(voltageChanged != -1){
//        handleVoltageChanged(voltageChanged);
//    }

    //TODO: calculate sleeping state based on time, motion, and doors
//    int sleepingChanged  = Device::getChangedValue("sleeping");
//
//    if( sleepingChanged != -1 ) {
//        handleSleepingChange(sleepingChanged);
//    }
}

//void handleVoltageChanged(int volts) {
//    // Do whatever is needed based on new volts value
//    if(volts >= 12) {
//        Log.info("Voltage is over 12: " + String(volts));
//    } else {
//        Log.info("Voltage is under 12: " + String(volts));
//    }
//}
