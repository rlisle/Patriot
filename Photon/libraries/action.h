/******************************************************************
 action class

 This class is the abstract parent of classes that support actions

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-24: Initial version
 ******************************************************************/

#ifndef action_h
#define action_h

#include "application.h"  // Defines String

class Action
{
public:
  virtual void set(String state) = 0;
  virtual void setOn() = 0;
  virtual void setOff() = 0;
  virtual bool isOn() = 0;
  virtual bool isOff() = 0;

};

#endif /* action_h */
