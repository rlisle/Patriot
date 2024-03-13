//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Couch (override)
//   LivingRoomMotion
//   Nighttime
//   RonHome
//   ShelleyHome
//   Sleeping
//------------

#include "couchCeilingLights.h"
#include "leftVerticalLights.h"

// Update all devices managed by this Photon2
void updateLights() {
    setCouchCeilingLights();
    setLeftVerticalLights();
}
