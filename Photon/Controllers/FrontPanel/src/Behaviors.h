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

// Update all devices managed by this Photon2
void updateLights() {
    PartOfDay pod = partOfDay();

    // Get pointers to all devices
    Device *cabinets = Device::get("Cabinets");
    Device *ceiling = Device::get("Ceiling");
    Device *kitchenCeiling = Device::get("KitchenCeiling");
    Device *leftTrim = Device::get("LeftTrim");
    Device *sinkLamp = Device::get("SinkLamp");
    Device *rightTrim = Device::get("RightTrim");

    Device *doorSide = Device::get("DoorSide");
    Device *frontAwning = Device::get("FrontAwning");
    Device *frontPorch = Device::get("FrontPorch");
    Device *otherSide = Device::get("OtherSide");

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        Device::setValue("Bedtime", 0);
        Device::setValue("Sleeping", 0);

        // Set lights
        kitchenCeiling->setValue(100);
        cabinets->setValue(100);
        sinkLamp->setValue(100);
        leftTrim->setValue(100);
        rightTrim->setValue(100);
        ceiling->setValue(100);
        if(pod == Evening) {
            doorSide->setValue(100);
            otherSide->setValue(100);
            frontAwning->setValue(100);
            frontPorch->setValue(100);
        }
        return;
    }

    switch(pod) {
        case Bedtime:
            // Turn off other statuses
            Device::setValue("Cleaning", 0);
            Device::setValue("Kitchen", 0);
            Device::setValue("Outside", 0);
            Device::setValue("Sink", 0);
            Device::setValue("Sleeping", 0);

            // Set lights
            kitchenCeiling->setValue(0);
            cabinets->setValue(0);
            sinkLamp->setValue(50);
            leftTrim->setValue(0);
            rightTrim->setValue(0);
            ceiling->setValue(0);
            doorSide->setValue(0);
            otherSide->setValue(0);
            frontAwning->setValue(0);
            frontPorch->setValue(0);
            break;
        case Sleeping:                  // Don't assume bedtime was set
            // Turn off other statuses
            Device::setValue("Bedtime", 0);
            Device::setValue("Cleaning", 0);
            Device::setValue("Kitchen", 0);
            Device::setValue("Outside", 0);
            Device::setValue("Sink", 0);

            // Set lights
            kitchenCeiling->setValue(0);
            cabinets->setValue(0);
            sinkLamp->setValue(0);
            leftTrim->setValue(0);
            rightTrim->setValue(0);
            ceiling->setValue(0);
            doorSide->setValue(0);
            otherSide->setValue(0);
            frontAwning->setValue(0);
            frontPorch->setValue(0);
            break;
        case AwakeEarly:
            sinkLamp->setValue(20);
            //TODO: turn on desk lamps
            break;
        case Morning:
        case Afternoon:
            kitchenCeiling->setValue(0);
            cabinets->setValue(0);
            sinkLamp->setValue(100);
            leftTrim->setValue(0);
            rightTrim->setValue(0);
            ceiling->setValue(0);
            doorSide->setValue(0);
            otherSide->setValue(0);
            frontAwning->setValue(0);
            frontPorch->setValue(0);
            break;
        case Evening:
            kitchenCeiling->setValue(20);
            // Don't set cabinets - they reflect off TV
            sinkLamp->setValue(50);
            leftTrim->setValue(100);
            rightTrim->setValue(100);
            ceiling->setValue(10);
            doorSide->setValue(0);
            otherSide->setValue(0);
            frontAwning->setValue(0);
            frontPorch->setValue(100);
            break;
    }
}

void setSinkOrOverride(int value) {
    if(is("Sink")) {
        set("SinkLamp", value("Sink"));
    }
    set("SinkLamp", value);
}
