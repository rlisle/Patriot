/**********************************************************************
 Inertial Measurement Unit Library

 This library provides a simple interface for using a LSM9DS1 device
 to track intertial movement. It include accelration, rotation, and
 magnetic measurements.

 **********************************************************************/

#ifndef imu_h
#define imu_h

#include "LSM9DS1.h"


class IMU
{
public:
  bool      isOnline;

  IMU();
  void  loop();
  void  setDelegate( void (*delegate)(IMU *imu) );

private:
  LSM9DS1   _imu;    //TODO: probably should rename to lsm9ds1
  long      _lastUpdateTime = 0;
  long      _lastPublishTime = 0;

  void  (*delegate)(IMU *imu);
  void  callDelegateIfSet();
  void  printGyro();
  void  printAccel();
  void  printMag();
  void  printAttitude(float ax, float ay, float az, float mx, float my, float mz);
};

#endif // imu_h
