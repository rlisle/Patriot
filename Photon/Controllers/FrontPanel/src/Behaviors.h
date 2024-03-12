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
#include "ceilingLights.h"
#include "kitchenCeilingLamps.h"
#include "leftTrimLights.h"
#include "nookLamp.h"
#include "rightTrimLights.h"
#include "sinkLamp.h"

// Update all devices managed by this Photon2
void updateLights() {

    setBedroomLamp();
    setCabinetLamps();
    setCeilingLights();
    setKitchenCeilingLamps();
    setLeftTrimLights();
    setNookLamp();
    setRightTrimLights();
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
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;

        case AwakeEarly:
            break;

        case Morning:
        case Afternoon:
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;

        case Evening:
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 100);
            break;
    }
}

