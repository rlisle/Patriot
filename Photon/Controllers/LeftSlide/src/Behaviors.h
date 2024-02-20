//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   LivingRoomMotion
//   Nighttime
//   RonHome
//   ShelleyHome
//   Sleeping
//------------

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

// Update all devices managed by this Photon2
void updateLights() {
    PartOfDay pod = partOfDay();

    // Get pointers to all devices
    Device *couchCeiling = Device::get("CouchCeiling");
    Device *leftVertical = Device::get("LeftVertical");

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        Device::setValue("Bedtime", 0);
        Device::setValue("Sleeping", 0);

        // Set lights
        couchCeiling->setValue(100);
        leftVertical->setValue(100);
        return;
    }

    switch(pod) {
        case Bedtime:
            // Turn off other statuses
            Device::setValue("Cleaning", 0);
            Device::setValue("Sleeping", 0);

            // Set lights
            couchCeiling->setValue(0);
            leftVertical->setValue(0);
            break;
            
        case Sleeping:                  // Don't assume bedtime was set
            // Turn off other statuses
            Device::setValue("Bedtime", 0);
            Device::setValue("Cleaning", 0);

            // Set lights
            couchCeiling->setValue(0);
            leftVertical->setValue(0);
            break;
            
        case AwakeEarly:
            //TODO: turn on nook lamp
            break;
            
        case Morning:
        case Afternoon:
            couchCeiling->setValue(0);
            leftVertical->setValue(0);
            break;
        case Evening:
            couchCeiling->setValue(20);
            leftVertical->setValue(20);
            break;
    }
}
