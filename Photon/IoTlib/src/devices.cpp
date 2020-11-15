/******************************************************************
Devices collection

This object manages a collection of device objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-11: Initial version
******************************************************************/
#include "devices.h"

Devices::Devices()
{
    // Without this method, strange error is reported and build fails
    _numDevices = 0;
    //TODO: dynamically allocate array space
    _devices = new Device *[MAX_NUM_DEVICES];
}

// Returns non-zero if # devices exceeded
int Devices::addDevice(Device *device)
{
    if (_numDevices < MAX_NUM_DEVICES - 1)
    {
        _devices[_numDevices++] = device;
    } else
    {
        return -1;
    }
    return 0;
}

// Called whenever "state" changes.
// The state that changed is passed to optimize handling.
// Devices can ignore the state change if they don't use it.
void Devices::stateDidChange(States *states) {
    for (int i = 0; i < _numDevices; i++)
    {
        _devices[i]->stateDidChange(states);
    }
}

void Devices::loop()
{
    for (int x = 0; x < _numDevices; x++)
    {
        _devices[x]->loop();
    }
}

Device *Devices::getDeviceByNum(int deviceNum)
{
    if (deviceNum < _numDevices)
    {
        return _devices[deviceNum];
    }
    return NULL;
}

Device *Devices::getDeviceWithName(String name)
{
    for (int i = 0; i < _numDevices; i++)
    {
        if (_devices[i]->name().equalsIgnoreCase(name))
        {
            return _devices[i];
        }
    }
    return NULL;
}

int Devices::numDevices()
{
    return _numDevices;
}
