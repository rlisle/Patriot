/**
Device parent class

This is the parent class of all device classes.
It provides static methods for managing the collection of devices.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#include "IoT.h"
#include "constants.h"

/**
 * globalStatesVariable and globalDevicesVariable
 * Lists all the currently active devices and their states names in CSV format.
 */
String globalStatesVariable;
String globalDevicesVariable;

Device::Device(String name)
: _next(NULL), _name(name), _value(0), _previous(-1)
{
    // Do any setup work in begin() not here.
}

void Device::setValue(int value) {
    Log.info("Device " + _name + " setValue " + String(value) + ", was "+String(_value));
    _value = value;
}

// Check if device has changed and return new value or -1
int  Device::getChangedValue() {
    if( _value == _previous ) return -1;
    _previous = _value;
    return _value;
}

void saveToPrevious() {
    _previous = _value;
}

void restorePrevious() {
    setValue(_previous);
}

// Static Methods

void Device::add(Device *device)
{
    Log.info("addDevice name: "+String(device->name()));
    if(_devices == NULL) {
        Log.info("  first device");
        _devices = device;
    } else {
        Device *ptr = _devices;
        int i = 1;
        while(ptr->_next != NULL) {
            ptr = ptr->_next;
            i++;
        }
        Log.info("  adding device "+String(i));
        ptr->_next = device;
    }
    device->begin();
    
    buildDevicesVariable();
}

void Device::resetAll()
{
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next)
    {
        // TODO: call reset only once per board, not device
        ptr->reset();
    }
}

void Device::loopAll()
{
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next)
    {
        ptr->loop();
    }
}

Device *Device::get(String name)
{
    Device *ptr = _devices;
    for (int i = 0; i < count() && ptr != NULL; i++)
    {
        if (ptr->name().equalsIgnoreCase(name)) {
            //Log.info("getDeviceWithName "+name+" found.");
            return ptr;
        }
        ptr = ptr->_next;
    }
    //Log.info("Device "+name+" not found, returning NULL");
    return NULL;
}

int Device::value(String name) {
    Device *ptr = get(name);
    if( ptr == NULL ) return -1;
    return( ptr->value() );
}

int Device::setValue(String name, int value) {
    Device *ptr = get(name);
    if( ptr == NULL ) return -1;
    ptr->setValue(value);
    return 0;
}

// Check if device has changed and return new value or -1
int  Device::getChangedValue(String name) {
    Device *device = get(name);
    if( device == NULL ) {
        Log.error("getChangedValue: " + name + " not found");
        return -1;
    }
    return device->getChangedValue();
}

int Device::count()
{
    int i = 0;
    for (Device *ptr = _devices; ptr != NULL; ptr = ptr->_next) i++;
    return i;
}

// Particle.io Devices variable

void Device::expose()
{
    if(!Particle.variable(kDevicesVariableName, globalDevicesVariable))
    {
        Log.error("Error: Unable to expose " + kDevicesVariableName + " variable");
    }
}

void Device::buildDevicesVariable()
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

// Publish each device name and its value
void Device::publishStates(String controllerName) {
    for (Device* ptr = _devices; ptr != NULL; ptr = ptr->_next) {
        IoT::mqttPublish("debug/"+controllerName+"/"+ptr->name()+"/state", String(ptr->_value));
    }
}
