/******************************************************************
Controller Names

This class represents all the controllers on the network.
These can be Lights, Fans, etc.
It keeps a list of all the controllers heard on the network,
and exposes one or more Particle.io string variables.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2017-01-10: Initial version
******************************************************************/
#include "IoT.h"
#include "controllernames.h"

String globalControllersVariable = "None";

ControllerNames::ControllerNames() {
    // Without this method, strange error is reported and build fails
    _numControllers = 0;
}

// Returns non-zero if # controllers exceeded
int ControllerNames::addController(String controller)
{
  IoT::log("addController("+controller+")");
  if(!this->doesNameExist(controller))
  {
    if (_numControllers < MAX_NUM_CONTROLLERNAMES-1)
    {
      _controllers[_numControllers++] = controller;
      this->buildControllersVariable();
      IoT::log("numControllers = "+String(_numControllers));
    } else {
      return -1;
    }
  }
  return 0;
}

bool ControllerNames::doesNameExist(String name)
{
  for(int i=0; i<_numControllers; i++)
  {
      if(_controllers[i].equalsIgnoreCase(name)) {
        return true;
      }
  }
  return false;
}

bool ControllerNames::expose()
{
  IoT::log("Expose controllerNames called");
  if(!Particle.variable(kControllersVariableName, globalControllersVariable))
  {
    IoT::log("Unable to expose "+String(kControllersVariableName)+" variable");
    return false;
  }
  return true;
}

void ControllerNames::buildControllersVariable()
{
  if(_numControllers > 0) {
    String newVariable = "";
    for(int i=0; i<_numControllers; i++)
    {
      newVariable += _controllers[i];
      if (i < _numControllers-1) {
        newVariable += ",";
      }
    }
    if(newVariable.length() < kMaxVariableStringLength) {
      globalControllersVariable = newVariable;
    } else {
      IoT::log("Controllers variable is too long. Need to extend to a 2nd variable");
    }
  }
}
