/******************************************************************
 motion control

 Features:
 - Reports motion

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-16: Initial version
 ******************************************************************/

#ifndef motion_h
#define motion_h

class Motion
{
public:

  Motion(int pinNum, const char *name);

  void    loop();

private:
  int     _pinNum;
  long    _lastChangeTime;
  bool    _wasDetected;
  
};

#endif /* motion_h */
