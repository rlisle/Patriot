//------------
// Behaviors
//------------

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

void updateLeftVertical(PartOfDay pod, bool isCleaning);
void updateCouch(PartOfDay pod, bool isCleaning);

void updateLights() {
    bool isCleaning = is("cleaning");
    PartOfDay pod = partOfDay();
    updateLeftVertical(pod, isCleaning);
    updateCouch(pod, isCleaning);
}

void updateLeftVertical(PartOfDay pod, bool isCleaning) {
    Device *device = Device::get("LeftVertical");
    if(isCleaning) {
        device->setValue(100);
    }
    switch(pod) {
        case Bedtime:
        case Sleeping:
            device->setValue(0);
            break;
        case AwakeEarly:
        case Morning:
        case Afternoon:
            break;
        case Evening:
            device->setValue(20);
            break;
    }
}

void updateCouch(PartOfDay pod, bool isCleaning) {
    Device *device = Device::get("Couch");
    if(isCleaning) {
        device->setValue(100);
    }
    switch(pod) {
        case Bedtime:
        case Sleeping:
            device->setValue(0);
            break;
        case AwakeEarly:
        case Morning:
        case Afternoon:
            break;
        case Evening:
            device->setValue(10);
            break;
    }
}
