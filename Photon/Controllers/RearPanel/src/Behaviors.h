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

#include "curtain.h"
#include "leftDeskLamp.h"
#include "loftLights.h"
#include "officeCeilingLights.h"
#include "pianoLight.h"
#include "rampAwningLight.h"
#include "rampPorchLight.h"
#include "rearAwningLight.h"
#include "rearPorchLight.h"
#include "rightDeskLamp.h"

// Update all devices managed by this Photon2
void updateLights() {
    setCurtain();
    setLeftDeskLamp();
    setLoftLights();
    setOfficeCeilingLights();
    setPianoLight();
    setRampAwningLight();
    setRampPorchLight();
    setRearAwningLight();
    setRearPorchLight();
    setRightDeskLamp();
}
