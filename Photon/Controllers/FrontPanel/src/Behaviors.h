//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Kitchen
//   LivingRoomMotion
//   Nighttime
//   Outside
//   RonHome
//   ShelleyHome
//   Sink
//   Sleeping
//------------
#define FRONT_DOOR_LIGHT_TIMEOUT 15*1000

#include "bedroomLamp.h"
#include "cabinetLamps.h"
#include "sinkLamp.h"

// void setBedroom(int percent) {
//     if(is("Bedroom")) {    // Is override value set?
//         Log.info("FP Bedroom overridden: %d", value("Bedroom"));
//         set("BedroomLamp", value("Bedroom"));
//     } else {
//         Log.info("FP BedroomLamp set to: %d", percent);
//         set("BedroomLamp", percent);
//     }
// }

// void setSink(int percent) {
//     if(is("Sink")) {    // Is override value set?
//         Log.info("FP Sink overridden: %d", value("Sink"));
//         set("SinkLamp", value("Sink"));
//     } else {
//         Log.info("FP sinkLamp set to: %d", percent);
//         set("SinkLamp", percent);
//     }
// }

void setKitchenCeiling(int percent) {
    if(is("Kitchen")) {
        set("KitchenCeiling", value("Kitchen"));
    } else {
        set("KitchenCeiling", percent);
    }
}

// void setCabinets(int percent) {
//     if(is("Cabinets")) {
//         set("CabinetLamps", value("Cabinets"));
//     } else {
//         set("CabinetLamps", percent);
//     }
// }

void setRightTrim(int percent) {
    if(is("Theatre")) {
        set("RightTrim", 0);
    } else {
        set("RightTrim", percent);
    }
}

void setNook(int percent) {
    if(is("Nook")) {
        Log.info("FP Nook overridden: %d", value("Nook"));
        set("NookLamp", value("Nook"));
    } else {
        Log.info("FP NookLamp set to: %d", percent);
        set("NookLamp", percent);
    }
}

// Update all devices managed by this Photon2
void updateLights() {

    setBedroomLamp();
    setCabinetLamps();
    setSinkLamp();

    PartOfDay pod = partOfDay();

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        Log.info("FP updateLights cleaning");

        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        // Set inside lights
//        setBedroom(100);
        setKitchenCeiling(100);
//        setCabinets(100);
//        setSink(100);
        set("LeftTrim", 100);
        set("RightTrim", 100);
        set("Ceiling", 100);
        set("NookLamp", 100);

        // Set outside lights TODO: update when door implemented
        if(pod == Evening) {
            set("DoorSide", 100);
            set("OtherSide", 100);
            set("FrontAwning", 100);
            set("FrontPorch", 100);
        }
        return;
    }

    switch(pod) {
        case Retiring:
            Log.info("FP updateLights retiring");

            // Turn off other statuses
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);
            set("Sleeping", 0);
            set("Theatre", 0);

            // Set lights
//            setBedroom(100);
            setKitchenCeiling(0);
//            setCabinets(0);
            setNook(33);
//            setSink(5);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;

        case Asleep:                  // Don't assume bedtime was set
            Log.info("FP updateLights asleep");

            // Turn off other statuses
            set("Bedroom", 0);
            set("Bedtime", 0);
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);
            set("Theatre", 0);

            // Set lights
//            setBedroom(0);
            setKitchenCeiling(0);
//            setCabinets(0);
            setNook(0);
//            setSink(0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;

        case AwakeEarly:
            Log.info("FP updateLights awake early");

//            setSink(5);
            setNook(30);
            break;

        case Morning:
        case Afternoon:
            Log.info("FP updateLights morning/afternoon");

//            setBedroom(0);
            setKitchenCeiling(0);
//            setCabinets(0);
//            setSink(0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            setNook(0);
            break;

        case Evening:
            Log.info("FP updateLights evening");

//            setBedroom(100);
            setKitchenCeiling(20);
            // Don't set cabinets unless overridden - they reflect off TV
//            setCabinets(0);
//            setSink(5);
            set("LeftTrim", 100);
            setRightTrim(100);
            set("Ceiling", 20);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 100);
            setNook(100);
            break;
    }
}

