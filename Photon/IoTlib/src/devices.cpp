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
        log("Error: Unable to expose " + kDevicesVariableName + " variable", LogError);
    }
}

void Devices::addDevice(Device *device)
{
    log("addDevice name: "+String(device->name()), LogDebug);
    if(_devices == NULL) {
        log("  first device", LogDebug);
        _devices = device;
    } else {
        log("  adding device", LogDebug);
        Device *ptr = _devices;
        while(ptr->_next != NULL) {
            ptr = ptr->_next;
        }
        ptr->_next = device;
    }
    buildDevicesVariable();
}

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
    for (int i = deviceNum; i > 0 && ptr->_next != NULL; i--) 
    {
        ptr = ptr->_next;
    }
    log("getDeviceByNum("+String(deviceNum)+" returning "+ptr->name());
    return ptr;
}

Device *Devices::getDeviceWithName(String name)
{
    Device *ptr = _devices;
    for (int i = 0; i < numDevices() && ptr != NULL; i++) 
    {
        if (ptr->name().equalsIgnoreCase(name)) {
            log("getDeviceWithName "+name+" found.", LogDebug);
            return ptr;
        }
        ptr = ptr->_next;
    }
    log("Device "+name+" not found, returning NULL", LogDebug);
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
        log("Devices variable is too long. Need to extend to a 2nd variable", LogError);
    }
}

void Devices::log(String message, PLogLevel logLevel) {
    IoT::getInstance()->log(message, logLevel);
}
