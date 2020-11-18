/******************************************************************
Devices collection

This object manages a collection of device objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-18: Convert to linked list
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-11: Initial version
******************************************************************/
#include "devices.h"

Devices::Devices()
{
    // Without this method, strange error is reported and build fails
    //TODO: dynamically allocate array space
    _devices = NULL;
}

void Devices::addDevice(Device *device)
{
    Serial.println("addDevice name: "+String(device->name()));
    if(_devices == NULL) {
        Serial.println("  first device");
        _devices = device;
    } else {
        Serial.println("  add device");
        Device *ptr = _devices;
        while(ptr->_next != NULL) {
            Serial.println("  advance to next");
            ptr = ptr->_next;
        }
        ptr->_next = device;
    }
}

// Called whenever "state" changes.
// The state that changed is passed to optimize handling.
// Devices can ignore the state change if they don't use it.
void Devices::stateDidChange(States *states) {
    for (int i = 0; i < numDevices(); i++)
    {
        getDeviceByNum(i)->stateDidChange(states);
    }
}

void Devices::loop()
{
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next)
    {
        ptr->loop();
    }
}

Device *Devices::getDeviceByNum(int deviceNum)
{
    Device *ptr = _devices;
    for (int i = deviceNum; i > 0 && ptr != NULL; i--) 
    {
        Serial.println("  getting next, current = "+ptr->name());
        ptr = ptr->_next;
    }
    Serial.println("Returning ptr to "+ptr->name());
    return ptr;
}

Device *Devices::getDeviceWithName(String name)
{
    Device *ptr = _devices;
    for (int i = 0; i < numDevices() && ptr != NULL; i++) 
    {
        if (ptr->name().equalsIgnoreCase(name)) {
            Serial.println("Returning ptr to "+ptr->name());
            return ptr;
        }
        Serial.println("  getting next, current = "+ptr->name());
        ptr = ptr->_next;
    }
    Serial.println("Not found, returning NULL");
    return NULL;
}

int Devices::numDevices()
{
    int i;
    Device *ptr = _devices;
    for (i = 0; ptr != NULL; i++) {
        ptr = ptr->_next;
    }
    return i;
}
