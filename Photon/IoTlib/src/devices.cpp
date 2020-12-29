/**
Devices collection

This object manages a collection of device objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#include "devices.h"
#include "constants.h"
#include "IoT.h"

String globalDevicesVariable;

Devices::Devices()
{
    _devices = NULL;
    expose();
}

void Devices::expose()
{
    if(!Particle.variable(kDevicesVariableName, globalDevicesVariable))
    {
        Log.error("Error: Unable to expose " + kDevicesVariableName + " variable");
    }
}

void Devices::addDevice(Device *device)
{
    Log.trace("addDevice name: "+String(device->name()));
    if(_devices == NULL) {
        Log.trace("  first device");
        _devices = device;
    } else {
        Log.trace("  adding device");
        Device *ptr = _devices;
        while(ptr->_next != NULL) {
            ptr = ptr->_next;
        }
        ptr->_next = device;
    }
    buildDevicesVariable();
}

void Devices::loop()
{
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next)
    {
        ptr->loop();
    }
}

void Devices::reset()
{
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next)
    {
        // TODO: call reset only once per board, not device
        ptr->reset();
    }
}


Device *Devices::getDeviceWithName(String name)
{
    Device *ptr = _devices;
    for (int i = 0; i < numDevices() && ptr != NULL; i++) 
    {
        if (ptr->name().equalsIgnoreCase(name)) {
            Log.trace("getDeviceWithName "+name+" found.");
            return ptr;
        }
        ptr = ptr->_next;
    }
    Log.trace("Device "+name+" not found, returning NULL");
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

//TODO: combine with addDevice
void Devices::buildDevicesVariable()
{
    String newVariable = "";
    
    for (Device* ptr = _devices; ptr != NULL; ptr = ptr->_next) {
        newVariable += ptr->_name;
        if (ptr->_next != NULL) {
            newVariable += ", ";
        }
    }
    if(newVariable.length() < kMaxVariableStringLength) {
        globalDevicesVariable = newVariable;
    } else {
        Log.error("Devices variable is too long. Need to extend to a 2nd variable");
    }
}
