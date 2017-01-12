/******************************************************************
 stepper motor control

 Features:
 - Open/Close control
 - Configurable step speed
 - Configurable # steps between open/close

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-09-04: Created from code used in PianoCabinet controller
 ******************************************************************/

#ifndef stepper_h
#define stepper_h

//TODO: move to instance variables
#define  kStepDelay              2        // 2ms per step
#define  kStepsPerRevolution     2048     // 32 steps * 64:1 gear ratio
#define  kNumRackTeeth           19 * 4   // 76
#define  kNumPinionTeeth         24
#define  kTotalRevolutions       kNumRackTeeth / kNumPinionTeeth // 3.167
#define  kTotalStepsWhenOpen     kStepsPerRevolution * kTotalRevolutions // 6,486

enum openState { closed, opening, open, closing };

class Stepper
{
private:
  int       _stepperNum;
  int       _pin1;
  int       _pin2;
  int       _pin3;
  int       _pin4;

  unsigned long   _currentTime;
  unsigned long   _lastStepTime;

  int             _targetStepCount;
  int             _currentStepCount;


public:
  int             percentOpen;
  openState       currentState;
  bool            isPerformingStartupCycle;

  Stepper(int stepperNum);
  void      open();
  void      close();
  void      loop();
  void      performStartupCycle();

};

#endif /* stepper_h */
