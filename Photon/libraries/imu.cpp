/**********************************************************************
 Inertial Measurement Unit Library

 This library provides a simple interface for using a LSM9DS1 device
 to track intertial movement. It include accelration, rotation, and
 magnetic measurements.

 **********************************************************************/

#include "math.h"
#include "application.h"
#include "imu.h"

#define kIMUUpdateInterval  1000  // 1 seconds
#define kIMUPublishInterval 15000 // 15 seconds

#define LSM9DS1_M  0x1E   // From SparkFun imu example
#define LSM9DS1_AG 0x6B
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250   // 250 ms between prints
// Latitude/Longitude in Austin 78724 is 30 17' 39" N, 97 37' 43" W
// Declination from http://www.ngdc.noaa.gov/geomag-web/#declination:
// 3 57' E +/- 0 20' changing by 0 7' W per year
#define DECLINATION -4.0


IMU::IMU(void) {
  isOnline = false;
  _lastUpdateTime = 0;
  _lastPublishTime = 0;

Particle.publish("DEBUG", "IMU constructor");
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  _imu.settings.device.commInterface = IMU_MODE_I2C;
  _imu.settings.device.mAddress = LSM9DS1_M;
  _imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!_imu.begin())
  {
    isOnline = false;
    Particle.publish("DEBUG", "IMU failed to begin");
  } else {
    isOnline = true;
    Particle.publish("DEBUG", "IMU initialized");
  }
}

void IMU::setDelegate( void (*callersDelegate)(IMU *imu) )
{
    delegate = callersDelegate;
}

void IMU::loop()
{
  if(isOnline) {
    long loopTime = millis();
    if(loopTime > _lastUpdateTime + kIMUUpdateInterval) {
      _imu.readGyro();
      _imu.readAccel();
      _imu.readMag();
      if(loopTime > _lastPublishTime + kIMUPublishInterval) {
        printGyro();
        printAccel();
        printMag();
        printAttitude(_imu.ax, _imu.ay, _imu.az, -_imu.my, -_imu.mx, _imu.mz);
        _lastPublishTime = loopTime;
      }
      _lastUpdateTime = loopTime;
    }
  }
}

void IMU::callDelegateIfSet() {
  if(delegate != NULL) {
    delegate(this);
  }
}

/***************/
/***   IMU   ***/
/***************/
void IMU::printGyro() {
  String gyro = "G: ";
  gyro += String(_imu.calcGyro(_imu.gx), 2);
  gyro += ", ";
  gyro += String(_imu.calcGyro(_imu.gy), 2);
  gyro += ", ";
  gyro += String(_imu.calcGyro(_imu.gz), 2);
  gyro += " deg/s";
  Particle.publish("DEBUG", gyro);
}

void IMU::printAccel() {
  String accel = "A: ";
  accel += String(_imu.calcAccel(_imu.ax), 2);
  accel += ", ";
  accel += String(_imu.calcAccel(_imu.ay), 2);
  accel += ", ";
  accel += String(_imu.calcAccel(_imu.az), 2);
  accel += " g";
  Particle.publish("DEBUG", accel);
}

void IMU::printMag() {
  String mag = "M: ";
  mag += String(_imu.calcMag(_imu.mx), 2);
  mag += ", ";
  mag += String(_imu.calcMag(_imu.my), 2);
  mag += ", ";
  mag += String(_imu.calcMag(_imu.mz), 2);
  mag += " g";
  Particle.publish("DEBUG", mag);
}

void IMU::printAttitude(float ax, float ay, float az, float mx, float my, float mz) {
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * M_PI / 180;

  if (heading > M_PI) heading -= (2 * M_PI);
  else if (heading < -M_PI) heading += (2 * M_PI);
  else if (heading < 0) heading += 2 * M_PI;

  // Convert everything from radians to degrees:
  heading *= 180.0 / M_PI;
  pitch *= 180.0 / M_PI;
  roll  *= 180.0 / M_PI;

  String attitude = "Pitch, Roll: ";
  attitude += String(pitch, 2);
  attitude += ", ";
  attitude += String(roll, 2);
  attitude += ", Heading: ";
  attitude += String(heading, 2);
  Particle.publish("DEBUG",attitude);
}
