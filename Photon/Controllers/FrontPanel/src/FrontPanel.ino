/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
  
  Converting to Photon 2 - "front_panel2"
  To update Photon:
   1. Edit this code
   2. Update IoT and plugins if needed
   3. Put Photon into listen mode using buttons
   4. "particle flash front_panel2 --target 5.4.1" or "ffp2"

  PHOTON 2 PINS
  RST - nc
  3V3 - nc
  Mode - nc
  Gnd - nc - 12v monitor R ladder -
  D11/A0/ADC4 - 12v monitor R ladder
  D12/A1/ADC5
  D13/A2/ADC3/PWM
  D14/A5/ADC0/PWM
  D19/S4
  D18/S3
  D17/SCK
  D15/MOSI/PWM - Ceiling
  D16/MISO/PWM - Cabinets
  D9/RX
  D8/TX
 
  LI+ - nc
  EN - nc
  VSUB (5v) - nc
  D10
  D7
  D6
  D5
  D4
  D3
  D2
  D1/A4/ADC1/PWM/Wire SCL
  D0/A3/ADC2/Wire SDA
 
  SWITCH WIRING
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

  TERMINAL STRIPS
   Top: LED Drivers
    *
    *
    *
    *
    *
    *
   Top 12v: Fuse #5 Purple/White
   Gnd
   Middle 12v: Fuse #2 Orange/White
   Bottom 12v: Fuse #14 Gray/White Awning
 
  I2CDIO8 Inputs
  1. Front Door switch
  2.
  3.
  4.
  5.
  6.
  7.
  8.
 
  Photon 2 is 0a10aced202194944a0446ac front_panel2
  RSSI = -63dBm on 9/17/23
 
  Original Photon was 430033000f47343339383037 FrontPanel
  RSSI = -66dBm  on 10/1/22
 
 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
 manually connect, but everything is automatic
 after that. This allows running loop and MQTT
 even if no internet available

 */
#include <IoT.h>

#define CONTROLLER_NAME "FrontPanel2"
#define MQTT_BROKER "192.168.50.33"
#define DIMMER_ADDRESS 0x41      // PWM board lowest switch on
#define PCA9634_ADDRESS 0x01     // Lowest switch on
//#define SWITCH_ADDRESS 0x20
//#define SWITCH_IOMAP 0xFF       // All 8 GPIOs are inputs

#define CURVE 2 // 0 = Linear, 1 = exponential, 2 = 50/50

// Until mystery hangs understood, leave in automatic
#define CONNECT_TO_CLOUD true
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

// Write Log messages to MQTT and/or serial port
#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_INFO);

int sleeping = 0;
int voltage = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
//    WiFi.useDynamicIP();
    
    // This also sets timezone and DST
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    // Set timezone and DST
    createDevices();
}

void createDevices() {

    // Required by NCD8Light
    int status = PCA9634::initialize(PCA9634_ADDRESS);
    if(status != 0) {
        Log.error("PCA9634 failed to initialize");
    }

    // Inside Lights
    Device::add(new NCD8Light(2, "KitchenCeiling", "Kitchen"));
    Device::add(new NCD8Light(3, "Sink", "Kitchen"));
    Device::add(new NCD8Light(5, "RightTrim", "Kitchen"));
    Device::add(new NCD8Light(6, "LeftTrim", "Living Room"));
    Device::add(new Light(D15, "Ceiling", "Kitchen"));
    Device::add(new Light(D16, "Cabinets", "Kitchen"));

    // Outside Lights
    Device::add(new NCD8Light(1, "DoorSide", "Outside"));
    Device::add(new NCD8Light(4, "OtherSide", "Outside"));
    Device::add(new NCD8Light(7, "FrontAwning", "Outside"));
    Device::add(new NCD8Light(8, "FrontPorch", "Outside"));

    // 12V Monitor (actually 14.27) with 10:1 R-Ladder
    // Adjust fullScale to reflect actual R-Ladder (36.9)
//    Device::add(new Voltage(A0, "FP volts", "LivingRoom", 36.9, 10));
    
//    Device::add(new Power("Power", "Status"));

    // Inputs - test/debug at this point
    
//    MCP23008::initialize(SWITCH_ADDRESS, SWITCH_IOMAP);

//    Device::add(new NCD8Switch(1, "FrontDoor", "Living Room"));
//    Device::add(new NCD8Switch(2, "Input2", "Living Room"));
//    Device::add(new NCD8Switch(3, "Input3", "Living Room"));
//    Device::add(new NCD8Switch(4, "Input4", "Living Room"));
//    Device::add(new NCD8Switch(5, "Input5", "Living Room"));
//    Device::add(new NCD8Switch(6, "Input6", "Living Room"));
//    Device::add(new NCD8Switch(7, "Input7", "Living Room"));
//    Device::add(new NCD8Switch(8, "Input8", "Living Room"));

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
