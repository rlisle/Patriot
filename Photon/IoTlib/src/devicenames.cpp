/******************************************************************
Device Names

This class represents all the devices on a controller.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle lib
2016-12-20: Initial version
******************************************************************/
#include "devicenames.h"
#include "constants.h"

String globalDevicesVariable;

DeviceNames::DeviceNames() {
    // Without this method, strange error is reported and build fails
    _numDevices = 0;
    expose();
}

void DeviceNames::expose()
{
    if(!Particle.variable(kDevicesVariableName, globalDevicesVariable))
    {
      Serial.print("Error: Unable to expose ");
      Serial.print(kDevicesVariableName);
      Serial.println(" variable");
    }
}

// Returns non-zero if # devices exceeded
int DeviceNames::addDevice(String device)
{
  if (_numDevices < MAX_NUM_DEVICENAMES-1)
  {
    _devices[_numDevices++] = device;
    buildDevicesVariable();
  } else {
    return -1;
  }
  return 0;
}

bool DeviceNames::doesNameExist(String name)
{
  for(int i=0; i<_numDevices; i++)
  {
      if(_devices[i].equalsIgnoreCase(name)) {
        return true;
      }
  }
  return false;
}

void DeviceNames::buildDevicesVariable()
{
  String newVariable = "";
  for(int i=0; i<_numDevices; i++)
  {
    newVariable += _devices[i];
    if (i < _numDevices-1) {
      newVariable += ",";
    }
  }
  if(newVariable.length() < kMaxVariableStringLength) {
    globalDevicesVariable = newVariable;
  } else {
    Serial.println("Devices variable is too long. Need to extend to a 2nd variable");
  }

}
