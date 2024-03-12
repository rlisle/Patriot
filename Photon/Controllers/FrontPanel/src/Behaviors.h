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
#include "doorSideLights.h"
#include "frontAwningLight.h"
#include "frontPorchLight.h"
#include "kitchenCeilingLamps.h"
#include "leftTrimLights.h"
#include "nookLamp.h"
#include "otherSideLights.h"
#include "rightTrimLights.h"
#include "sinkLamp.h"

// Update all devices managed by this Photon2
void updateLights() {

    setBedroomLamp();
    setCabinetLamps();
    setCeilingLights();
    setDoorSideLights();
    setFrontAwningLight();
    setFrontPorchLight();
    setKitchenCeilingLamps();
    setLeftTrimLights();
    setNookLamp();
    setOtherSideLights();
    setRightTrimLights();
    setSinkLamp();

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        Log.info("FP updateLights cleaning");

        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);
        return;
    }

    switch(partOfDay()) {
        case Retiring:
            Log.info("FP updateLights retiring");

            // Turn off other statuses
            set("Bedroom", 0);
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);
            set("Theatre", 0);
            break;

        case Asleep:                  // Don't assume bedtime was set
            Log.info("FP updateLights asleep");
            set("Bedroom", 0);
            set("Retiring", 0);
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);
            set("Theatre", 0);
            break;

        case AwakeEarly:
        case Morning:
        case Afternoon:
        case Evening:
            break;
    }
}

