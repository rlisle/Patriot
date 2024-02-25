//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Desk
//   LivingRoomMotion
//   Nighttime
//   Office
//   Outside
//   RonHome
//   ShelleyHome
//   Sleeping
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

// Inside Overrides
void setOfficeCeiling(int value) {
    if(is("Cleaning")) {
        set("OfficeCeiling", 100);
    } else if(is("Office")) {
        set("OfficeCeiling", value("Office"));
    } else {
        set("OfficeCeiling", value);
    }
}

void setLoft(int value) {
    if(is("Cleaning")) {
        set("Loft", 100);
    } else {
        set("Loft", value);
    }
}

void setPiano(int value) {
    if(is("Cleaning")) {
        set("Piano", 100);
    } else if(is("Office")) {
        set("Piano", value("Office"));
    } else {
        set("Piano", value);
    }
}

// Outside Overrides
void setRearPorch(int value) {
    if(is("Outside") || is("OfficeDoor") || isTimingOfficeDoor) {
        set("RearPorch", 100);
    } else {
        set("RearPorch", value);
    }
}

void setRearAwning(int value) {
    if(is("Outside") || is("OfficeDoor") || isTimingOfficeDoor) {
        set("RearAwning", 100);
    } else {
        set("RearAwning", value);
    }
}

void setRampPorch(int value) {
    if(is("Outside")) {
        set("RampPorch", 100);
    } else {
        set("RampPorch", value);
    }
}

void setRampAwning(int value) {
    if(is("Outside")) {
        set("RampAwning", 100);
    } else {
        set("RampAwning", value);
    }
}

// Update all devices managed by this Photon2
void updateLights() {
    Log.info("RP updateLights");

    switch(partOfDay()) {
        case Asleep:
            Log.info("RP updateLights Asleep");
            set("Curtain", 0);     // Close curtain

            setLoft(0);
            setOfficeCeiling(0);
            setPiano(0);

            setRampPorch(0);
            setRampAwning(0);
            setRearAwning(0);
            setRearPorch(0);
            break;

        case AwakeEarly:
            Log.info("RP updateLights AwakeEarly");
            setLoft(0);
            setOfficeCeiling(5);
            setPiano(5);

            setRampPorch(0);
            setRampAwning(0);
            setRearAwning(0);
            setRearPorch(0);
            break;

        case Morning:
        case Afternoon:
            Log.info("RP updateLights daytime");
            setLoft(0);
            setOfficeCeiling(0);
            setPiano(100);

            setRampPorch(0);
            setRampAwning(0);
            setRearAwning(0);
            setRearPorch(0);
            break;

        case Evening:
            Log.info("RP updateLights evening");
            setLoft(0);
            setOfficeCeiling(5);
            setPiano(50);

            setRampPorch(100);
            setRampAwning(100);
            setRearAwning(100);
            setRearPorch(100);
            break;

        case Retiring:
            Log.info("RP updateLights retiring");

            set("Curtain", 0);     // Close curtain
            
            setLoft(0);
            setOfficeCeiling(5);
            setPiano(5);

            setRampPorch(0);
            setRampAwning(0);
            setRearAwning(0);
            setRearPorch(0);
            break;
    }
}
