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
    NCD8Light *odsFloods = new NCD8Light(ADDRESS2, 3, "OtherSide");
    NCD8Light *rightTrim = new NCD8Light(ADDRESS2, 4, "RightTrim",1);
    NCD8Light *leftTrim = new NCD8Light(ADDRESS2, 5, "LeftTrim",1);
    NCD8Light *frontAwning = new NCD8Light(ADDRESS2, 6, "FrontAwning");
    NCD8Light *frontPorch = new NCD8Light(ADDRESS2, 7, "FrontPorch");

    Light *ceiling = new Light(D2, "Ceiling", 2);
    Light *cabinets = new Light(D3, "Cabinets", 2);

    // Switch wiring
    // Top left: RX     tape "Cabinet"
    // Top right: TX
    // 2nd left: A7     tape "Sink"
    // 2nd middle: A5   tape "Ceiling"
    // 2nd right: A6    tape "Indirect"
    // 3rd left: A1     "Ceiling"
    // 3rd l-m: A2      "DS Flood Lights"
    // 3rd r-m: A3      "ODS Flood Lights"
    // 3rd right: A4    "Porch Lights"
    // bottom: A0       "Light"
    Switch *cabinetSwitch = new Switch(A5, "CabinetSwitch");            // tape "Cabinet"     (top left)
    Switch *rightTrimSwitch = new Switch(A6, "RightTrimSwitch");        //                    (top right)
    Switch *sinkSwitch = new Switch(A7, "SinkSwitch");                  // tape "Sink"        (2nd left)
    Switch *kitchenCeilingSwitch = new Switch(RX, "KitchenCeilingSwitch"); // tape "Ceiling"  (2nd middle)
    Switch *leftTrimSwitch = new Switch(TX, "LeftTrimSwitch");          // tape "Indirect"    (2nd right)
    
    Switch *ceilingSwitch = new Switch(A1, "CeilingSwitch");            // "Ceiling"          (3rd left)
    Switch *dsFloodsSwitch = new Switch(A2, "DoorSideSwitch");          // "DS Flood Lights"  (3rd l-m)
    Switch *odsFloodsSwitch = new Switch(A3, "OtherSideSwitch");        // "ODS Flood Lights" (3rd m-r)
    Switch *frontPorchSwitch = new Switch(A4, "FrontPorchSwitch");      // "Porch Lights"     (3rd right)
    Switch *frontAwningSwitch = new Switch(A0, "FrontAwningSwitch");    // "Light"            (bottom)

    // ACTIVITIES - none (see RearPanel)
        
    // DEVICES

    iot->addDevice(kitchenCeiling);
    iot->addDevice(sink);
    iot->addDevice(rightTrim);
    iot->addDevice(leftTrim);
    iot->addDevice(dsFloods);
    iot->addDevice(odsFloods);
    iot->addDevice(frontAwning);
    iot->addDevice(frontPorch);

    iot->addDevice(ceiling);
    iot->addDevice(cabinets);

    iot->addDevice(ceilingSwitch);
    iot->addDevice(kitchenCeilingSwitch);
    iot->addDevice(sinkSwitch);
    iot->addDevice(cabinetSwitch);
    iot->addDevice(rightTrimSwitch);
    iot->addDevice(leftTrimSwitch);
    iot->addDevice(dsFloodsSwitch);
    iot->addDevice(odsFloodsSwitch);
    iot->addDevice(frontPorchSwitch);
    iot->addDevice(frontAwningSwitch);

}

// Save previous states we care about
int prevSleeping = ASLEEP;
int prevPartOfDay = NIGHT;

int prevCeilingSwitch = 0;
int prevKitchenCeilingSwitch = 0;
int prevSinkSwitch = 0;
int prevCabinetSwitch = 0;
int prevRightTrimSwitch = 0;
int prevLeftTrimSwitch = 0;
int prevDoorSideSwitch = 0;
int prevOtherSideSwitch = 0;
int prevFrontPorchSwitch = 0;
int prevFrontAwningSwitch = 0;

void loop() {
    int sleeping = iot->getState("sleeping");
    int partOfDay = iot->getState("partofday");
    
    int ceilingSwitch = iot->getState("CeilingSwitch");
    int kitchenCeilingSwitch = iot->getState("KitchenCeilingSwitch");
    int sinkSwitch = iot->getState("SinkSwitch");
    int cabinetSwitch = iot->getState("CabinetSwitch");
    int rightTrimSwitch = iot->getState("RightTrimSwitch");
    int leftTrimSwitch = iot->getState("LeftTrimSwitch");
    int doorSideSwitch = iot->getState("DoorSideSwitch");
    int otherSideSwitch = iot->getState("OtherSideSwitch");
    int frontPorchSwitch = iot->getState("FrontPorchSwitch");
    int frontAwningSwitch = iot->getState("FrontAwningSwitch");

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
            iot->setDevice("FrontAwning", 100);
            iot->setDevice("FrontPorch", 60);
        }
        
        prevPartOfDay = partOfDay;
    }
    
    if( ceilingSwitch != prevCeilingSwitch) {
        iot->setDevice("Ceiling", ceilingSwitch);
        prevCeilingSwitch = ceilingSwitch;
    }
    if( kitchenCeilingSwitch != prevKitchenCeilingSwitch) {
        iot->setDevice("KitchenCeiling", kitchenCeilingSwitch);
        prevKitchenCeilingSwitch = kitchenCeilingSwitch;
    }
    if( sinkSwitch != prevSinkSwitch) {
        iot->setDevice("Sink", sinkSwitch);
        prevSinkSwitch = sinkSwitch;
    }
    if( cabinetSwitch != prevCabinetSwitch) {
        iot->setDevice("Cabinet", cabinetSwitch);
        prevCabinetSwitch = cabinetSwitch;
    }
    if( rightTrimSwitch != prevRightTrimSwitch) {
        iot->setDevice("RightTrim", rightTrimSwitch);
        prevRightTrimSwitch = rightTrimSwitch;
    }
    if( leftTrimSwitch != prevLeftTrimSwitch) {
        iot->setDevice("LeftTrim", leftTrimSwitch);
        prevLeftTrimSwitch = leftTrimSwitch;
    }
    if( doorSideSwitch != prevDoorSideSwitch) {
        iot->setDevice("DoorSide", doorSideSwitch);
        prevDoorSideSwitch = doorSideSwitch;
    }
    if( otherSideSwitch != prevOtherSideSwitch) {
        iot->setDevice("OtherSide", otherSideSwitch);
        prevOtherSideSwitch = otherSideSwitch;
    }
    if( frontPorchSwitch != prevFrontPorchSwitch) {
        iot->setDevice("FrontPorch", frontPorchSwitch);
        prevFrontPorchSwitch = frontPorchSwitch;
    }
    if( frontAwningSwitch != prevFrontAwningSwitch) {
        iot->setDevice("FrontAwning", frontAwningSwitch);
        prevFrontAwningSwitch = frontAwningSwitch;
    }

    iot->loop();
}

// This method will read a switch, and set the associated
// device when it changes. The switch and device must use
// similar names, the "Switch" appended to the device name.
// TODO: move this into IoT once working
//void setDeviceFromSwitch(String name, int *prevValue) {
//    int switchValue = iot->getState(name+"Switch");
//    //TODO:
//}

void setAllInsideLights(int level) {
    iot->setDevice("KitchenCeiling", level);
    iot->setDevice("Sink", level);
    iot->setDevice("RightTrim", level);
    iot->setDevice("LeftTrim", level);
    iot->setDevice("Ceiling", level);
    iot->setDevice("Cabinets", level);
}

void setAllOutsideLights(int level) {
    iot->setDevice("DoorSide", level);
    iot->setDevice("OtherSide", level);
    iot->setDevice("FrontAwning", level);
    iot->setDevice("FrontPorch", level);
}
