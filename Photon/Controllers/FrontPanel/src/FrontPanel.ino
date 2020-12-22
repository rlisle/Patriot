/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
 
 This isn't the bridge controller, so we shouldn't receive any activities from particle.io
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    2. "particle flash FrontPanel"
 
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define ADDRESS2 1      // PWM board switches low switch on

String mqttServer = "192.168.10.184";

IoT *iot;

              
void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("FrontPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "patriotFrontPanel1");

    NCD8Light *dsFloods = new NCD8Light(ADDRESS2, 0, "DoorSide");
    NCD8Light *kitchenCeiling = new NCD8Light(ADDRESS2, 1, "kitchenCeiling", 2);
    NCD8Light *sink = new NCD8Light(ADDRESS2, 2, "Sink", 2);
    NCD8Light *osFloods = new NCD8Light(ADDRESS2, 3, "OtherSide");
    NCD8Light *rightTrim = new NCD8Light(ADDRESS2, 4, "RightTrim",1);
    NCD8Light *leftTrim = new NCD8Light(ADDRESS2, 5, "LeftTrim",1);
    NCD8Light *frontAwning = new NCD8Light(ADDRESS2, 6, "FrontAwning");
    NCD8Light *frontPorch = new NCD8Light(ADDRESS2, 7, "FrontPorch");

    Light *ceiling = new Light(D2, "ceiling", 2);
    Light *cabinets = new Light(D3, "cabinets", 2);

    // Enable and reorder once wiring connected
    //Switch *ceilingSwitch = new Switch(A0, "CeilingSwitch");
    //Switch *kitchenCeilingSwitch = new Switch(A1, "KitchenCeilingSwitch");
    //Switch *sinkSwitch = new Switch(A2, "SinkSwitch");
    //Switch *cabinetSwitch = new Switch(A3, "CabinetSwitch");
    //Switch *rightTrimSwitch = new Switch(A4, "RightTrimSwitch");
    //Switch *leftTrimSwitch = new Switch(A5, "LeftTrimSwitch");
    //Switch *dsFloodsSwitch = new Switch(A6, "DSFloodsSwitch");
    //Switch *osFloodsSwitch = new Switch(A7, "ODSFloodsSwitch");
    //Switch *frontPorchSwitch = new Switch(RX, "FrontPorchSwitch");
    //Switch *frontAwningSwitch = new Switch(TX, "FrontAwningSwitch");

    // ACTIVITIES - none (see RearPanel)
        
    // Switches
// Uncomment these once they're hooked up. Otherwise they appear to be ON
//    ceiling->addBehavior(100, "CeilingSwitch", '>', 0);
//    kitchenCeiling->addBehavior(100, "KitchenCeilingSwitch", '>', 0);
//    sink->addBehavior(100, "SinkSwitch", '>', 0);
//    kitchenCabinets->addBehavior(100, "CabinetSwitch", '>', 0);
//    rightTrim->addBehavior(100, "RightTrimSwitch", '>', 0);
//    leftTrim->addBehavior(100, "LeftTrimSwitch", '>', 0);
//    dsFloods->addBehavior(100, "DSFloodsSwitch", '>', 0);
//    osFloods->addBehavior(100, "OSFloodsSwitch", '>', 0);
//    frontPorch->addBehavior(100, "FrontPorchSwitch", '>', 0);
//    frontAwning->addBehavior(100, "FrontAwningSwitch", '>', 0);

    // DEVICES

    iot->addDevice(kitchenCeiling);
    iot->addDevice(sink);
    iot->addDevice(rightTrim);
    iot->addDevice(leftTrim);
    iot->addDevice(dsFloods);
    iot->addDevice(osFloods);
    iot->addDevice(frontAwning);
    iot->addDevice(frontPorch);

    iot->addDevice(ceiling);
    iot->addDevice(cabinets);

//    iot->addDevice(ceilingSwitch);
//    iot->addDevice(kitchenCeilingSwitch);
//    iot->addDevice(sinkSwitch);
//    iot->addDevice(cabinetSwitch);
//    iot->addDevice(rightTrimSwitch);
//    iot->addDevice(leftTrimSwitch);
//    iot->addDevice(dsFloodsSwitch);
//    iot->addDevice(osFloodsSwitch);
//    iot->addDevice(frontPorchSwitch);
//    iot->addDevice(frontAwningSwitch);

}

// Save previous states we care about
int prevSleeping = ASLEEP;
int prevPartOfDay = NIGHT;

void loop() {
    int sleeping = iot->getState("sleeping");
    int partOfDay = iot->getState("partofday");
    
    if( sleeping != prevSleeping ) {
        
        // Alexa, Good morning
        if( sleeping == AWAKE && partOfDay > SUNSET ) {
            iot->setDevice("ceiling", 40);
            iot->setDevice("kitchenCeiling", 40);
            iot->setDevice("cabinets", 50);
            iot->setDevice("sink", 60);
        }
        
        // Alexa, bedtime
        if( sleeping == ASLEEP) {
            iot->setDevice("ceiling", 30);
            iot->setDevice("kitchenCeiling", 30);
            iot->setDevice("cabinets", 30);
            iot->setDevice("sink", 30);
        }
        
        // Alexa, Good Night
        if( sleeping == ASLEEP) {
            iot->setDevice("ceiling", 0);
            iot->setDevice("kitchenCeiling", 0);
            iot->setDevice("cabinets", 0);
            iot->setDevice("sink", 0);
        }

        prevSleeping = sleeping; // Refactor to IoT
    }
    
    if( partOfDay != prevPartOfDay) {
        
        // Turn off lights at sunrise
        if( partOfDay == SUNRISE ) {
            iot->setDevice("ceiling", 0);
            iot->setDevice("kitchenCeiling", 0);
            iot->setDevice("cabinets", 0);
            iot->setDevice("sink", 0);
        }
        
        // Turn on lights in the evening
        if( partOfDay == DUSK ) {
            iot->setDevice("ceiling", 60);
            iot->setDevice("kitchenCeiling", 60);
            iot->setDevice("cabinets", 60);
            iot->setDevice("sink", 60);
        }
        
        prevPartOfDay = partOfDay;
    }
    

    iot->loop();
}
