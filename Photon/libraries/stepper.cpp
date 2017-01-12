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

#include "stepper.h"
#include "LisleRV.h"

Stepper::Stepper(int stepperNum)
{
  _stepperNum = stepperNum;
  if(stepperNum==0) {   // Currently only 2 sets of stepper pins supported
    _pin1 = kStepper1Pin1;
    _pin2 = kStepper1Pin2;
    _pin3 = kStepper1Pin3;
    _pin4 = kStepper1Pin4;
  } else {
    _pin1 = kStepper2Pin1;
    _pin2 = kStepper2Pin2;
    _pin3 = kStepper2Pin3;
    _pin4 = kStepper2Pin4;
  }
  pinMode(_pin1, OUTPUT);
  pinMode(_Pin2, OUTPUT);
  pinMode(_Pin3, OUTPUT);
  pinMode(_Pin4, OUTPUT);
  openState = closed;
  shutOffMotors();
}

void open() {
  if(_currentStepCount == kTotalStepsWhenOpen) {
      Serial.println("Stepper already open");
  } else if(_targetStepCount == kTotalStepsWhenOpen) {
      Serial.println("Open already in progress");
  } else {
      Serial.println("Stepper opening");
      currentState    = opening;
      _targetStepCount = kTotalStepsWhenOpen;
  }
}

void close() {
    if(_currentStepCount == 0) {
        Serial.println("Stepper already closed");
    } else if(_targetStepCount == 0) {
        Serial.println("Close already in progress");
    } else {
        Serial.println("Stepper closing");
        currentState = closing;
        _targetStepCount = 0;
    }
}

void performStartupCycle() {
    isPerformingStartupCycle = true;
    open();
}

void loop() {
  _currentTime = millis();
  if(_currentTime < _lastStepTime + kStepDelay) return;
  if(_currentStepCount == _targetStepCount) return;

  if(_targetStepCount > _currentStepCount) {
    _currentStepCount++;
  } else {
    _currentStepCount--;
  }

  nextStep();

  if(_currentStepCount == 0) {
    Serial.println("Stepper closed");
    currentState = closed;
    percentOpen  = 0;
    shutOffMotors();

  } else if(_currentStepCount == _targetStepCount) {
    Serial.println("Stepper open");
    currentState = open;
    percentOpen  = 100;

    if (performStartupCycle) {
        closePiano();
        performStartupCycle = false;
    } else {
        shutOffMotors();
    }

  } else {
    percentOpen = calculatePercentOpen();
  }

  _lastStepTime = _currentTime;
}

void nextStep()
{
  switch(_currentStepCount % 4)
  {
    case 0:
    //  printf("Step 1 - blue, pink on");
      digitalWrite(_pin4, LOW);
      digitalWrite(_pin2, HIGH);
      break;
    case 1:
      //  printf("Step 2 - pink, yellow on");
      digitalWrite(_pin1, LOW);
      digitalWrite(_pin3, HIGH);
      break;
    case 2:
      //  printf("Step 3 - yellow, orange on");
      digitalWrite(_pin2, LOW);
      digitalWrite(_pin4, HIGH);
      break;
    case 3:
    //  printf("Step 4 - orange, blue on");
      digitalWrite(_pin3, LOW);
      digitalWrite(_pin1, HIGH);
      break;
    }
}

void shutOffMotors() {
    Serial.println("Shut off motors");
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    digitalWrite(_pin3, LOW);
    digitalWrite(_pin4, LOW);
}

int calculatePercentOpen() {
    if(_currentStepCount == 0) {
        return 0;
    }
    long t = _currentStepCount * 100;
    t /= kTotalStepsWhenOpen;
    return (int)t;
}
