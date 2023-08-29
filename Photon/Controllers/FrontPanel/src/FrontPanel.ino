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

#define CONTROLLER_NAME "FrontPanel"
#define MQTT_BROKER "192.168.50.33"
#define DIMMER_ADDRESS 0x41      // PWM board lowest switch on
#define SWITCH_ADDRESS 0x20
#define SWITCH_IOMAP 0xFF       // All 8 GPIOs are inputs

#define VMPIN A0

// Until mystery hangs understood, leave in automatic
#define CONNECT_TO_CLOUD true
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define MQTT_LOGGING false
SerialLogHandler logHandler1(57600, LOG_LEVEL_INFO);

int sleeping = 0;
int voltage = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    createDevices();
}

void createDevices() {

    PCA9685::initialize(DIMMER_ADDRESS);

    // Inside Lights
    Device::add(new NCD16Light(1, "KitchenCeiling2", "Kitchen", 2));
    Device::add(new NCD16Light(2, "Sink2", "Kitchen", 2));
    Device::add(new NCD16Light(4, "RightTrim2", "Kitchen", 2));
    Device::add(new NCD16Light(5, "LeftTrim2", "Living Room", 2));
    Device::add(new NCD16Light(6, "Ceiling2", "Kitchen", 2));
    Device::add(new NCD16Light(7, "Cabinets2", "Kitchen", 2));

    // Outside Lights
    Device::add(new NCD16Light(8, "DoorSide2", "Outside", 2));
    Device::add(new NCD16Light(9, "OtherSide2", "Outside", 2));
    Device::add(new NCD16Light(10, "FrontAwning2", "Outside", 2));
    Device::add(new NCD16Light(11, "FrontPorch2", "Outside", 2));

    //TODO: I/O pins not available. Will need a separate NCD board
    // 12V Monitor (actually 14.27) with 10:1 R-Ladder
    // Adjust fullScale to reflect actual R-Ladder (36.9)
//    Device::add(new Voltage(A0, "FP volts", "LivingRoom", 36.9, 10));
    
//    Device::add(new Power("Power", "Status"));

    // Inputs - test/debug at this point
    
    MCP23008::initialize(SWITCH_ADDRESS, SWITCH_IOMAP);
    
    Device::add(new NCD8Switch(1, "Input1", "Living Room"));
    Device::add(new NCD8Switch(2, "Input2", "Living Room"));
    Device::add(new NCD8Switch(3, "Input3", "Living Room"));
    Device::add(new NCD8Switch(4, "Input4", "Living Room"));
    Device::add(new NCD8Switch(5, "Input5", "Living Room"));
    Device::add(new NCD8Switch(6, "Input6", "Living Room"));
    Device::add(new NCD8Switch(7, "Input7", "Living Room"));
    Device::add(new NCD8Switch(8, "Input8", "Living Room"));

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
