//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Cleaning
//   Desk
//   Loft
//   Nighttime
//   Office
//   OfficeMotion
//   Outside
//   Piano
//   Retiring (bedtime)
//   RonHome
//   ShelleyHome
//   Sleeping
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

#include "leftDeskLamp.h"
#include "loftLights.h"
#include "officeCeilingLights.h"
#include "pianoLight.h"
#include "rampAwningLight.h"
#include "rampPorchLight.h"
#include "rearAwningLight.h"
#include "rearPorchLight.h"
#include "rightDeskLamp.h"

// Outside Overrides

// Update all devices managed by this Photon2
void updateLights() {

    setLeftDeskLamp();
    setLoftLights();
    setOfficeCeilingLights();
    setPianoLight();
    setRampAwningLight();
    setRampPorchLight();
    setRearAwningLight();
    setRearPorchLight();
    setRightDeskLamp();

    switch(partOfDay()) {
        case Asleep:
            set("Curtain", 0);     // Close curtain
            break;

        case AwakeEarly:
            break;

        case Morning:
        case Afternoon:
            break;

        case Evening:
            break;

        case Retiring:
            set("Curtain", 0);     // Close curtain
            break;
    }
}
