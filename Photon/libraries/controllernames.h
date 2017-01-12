/******************************************************************
 Device Names

 This class represents all the controllers on the network.
 These can be Lights, Fans, etc.
 It keeps a list of all the controllers heard on the network,
 and exposes one or more Particle.io string variables.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-12-20: Initial version
 ******************************************************************/

#ifndef controllernames_h
#define controllernames_h

#include "application.h"  // Defines String

#define MAX_NUM_CONTROLLERNAMES 32

class ControllerNames
{
public:

  ControllerNames();

  int         addController(String controller);
  bool        doesNameExist(String name);
  bool        expose();

private:
  uint8_t     _numControllers;
  String      _controllers[MAX_NUM_CONTROLLERNAMES];

  void buildControllersVariable();
};

#endif /* controllernames_h */
