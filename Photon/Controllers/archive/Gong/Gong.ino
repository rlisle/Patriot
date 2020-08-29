/**
 * Gong
 *
 * This sketch runs on a Photon mounted inside a gong mallet head.
 *
 * In the future, it may be automatically turned on and off
 * by a set of tilt switches when the mallet is picked up.
 * For now, there will be a switch mounted to the bottom of the head.
 *
 * It monitors accelerometer data from the Sparkfun Photon IMU Shield
 * using the LSM9DS1 library. When the data exceeds a threshold,
 * it sends an event to another device to play a gong sound.
 *
 * Since the Photon has built-in Wifi, it uses that to communicate to
 * the other device.
 *
 * Hardware
 * 1. Photon 3.3v
 * 2. Sparkfun IMU Shield (9 axis: accelerometer, gyro, magnetometer) (D0, D1)
 * 3. 9v battery
 * 4. On/Off switch
 * 5. 3.3v buck regulator
 * 6. Red LED indicates when power is on (pin 6)
 *    Blinking red during power up
 *    Steady red once ready to play (Wifi connection established, etc)
 * 7. Green LED indicates when strike detected (pin 7) (Also built-in LED)
 */

byte me;  // #defines confuse the IDE. This line at top unconfuses it.

/****************/
/*** INCLUDES ***/
/****************/
#include "LSM9DS1.h"
#include "math.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define NumberOfMillisInSecond  1000
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

#define kStrikeLedPin    7
#define kPowerLedPin     6

#define kDisplayInterval 250  // Wait between display outputs
#define kRestrikeDelay   500  // Wait at least half a second before retriggering

#define kDeclination     -3.9 // Declination in Boulder, CO
                                // Find it at http://www.ngdc.noaa.gov/geomag-web/#declination

#define kLedHoldTime     1000  // Hold strike LED on for 1/4 second
#define kThreshold       1.0  // TBD

/*****************/
/*** VARIABLES ***/
/*****************/
LSM9DS1 imu;

long    loopTime           = 0;
long    lastDisplayTime    = 0;
long    lastStrikeTime     = 0;
bool    ledIsOn            = false;
float   previousAX         = 0.0;
float   previousAY         = 0.0;
float   previousAZ         = 0.0;

/*************/
/*** SETUP ***/
/*************/
void setup() {
  setupSerial();
  setupPins();
  setupIMU();
}

void setupSerial() {
  Serial.begin(57600);
  while(!Serial);
  Serial.println("Starting Gong Mallet controller...");
}

void setupPins(void) {
  pinMode(kStrikeLedPin,   OUTPUT);
  pinMode(kPowerLedPin,    OUTPUT);
}

void setupIMU() {
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  if (!imu.begin()) {
    Particle.publish("DEBUG","Failed to communicate with IMU");
    while(1);
  }
}

/************/
/*** LOOP ***/
/************/
void loop() {
  loopTime = millis();
  readIMU();
  displayIMU();
  turnOffStrikeLed();
}

// Shock switch detects when gong struck
void readIMU() {

  imu.readAccel();
  float ax = imu.calcAccel(imu.ax);
  float ay = imu.calcAccel(imu.ay);
  float az = imu.calcAccel(imu.az);

  if(fabs(previousAX - ax) > kThreshold) {
    Serial.print("AX exceeded threshold: ");
    Serial.print(ax,2);
    handleStrike();
  }
  if(fabs(previousAY - ay) > kThreshold) {
    Serial.print("AY exceeded threshold: ");
    Serial.println(ay,2);
    handleStrike();
  }
  if(fabs(previousAZ - az) > kThreshold) {
    Serial.print("AZ exceeded threshold: ");
    Serial.println(az,2);
    handleStrike();
  }
  previousAX = ax;
  previousAY = ay;
  previousAZ = az;

}

void displayIMU() {

  if (loopTime >= lastDisplayTime + kDisplayInterval) {
    lastDisplayTime = loopTime;

    Serial.print("X,Y,Z = ");
    Serial.print(previousAX,2);
    Serial.print(",");
    Serial.print(previousAY,2);
    Serial.print(",");
    Serial.println(previousAZ,2);

//    printGyro();
//    printAccel();
//    printMag();

//    printAttitude(imu.ax, imu.ay, imu.az, -imu.my, -imu.mx, imu.mz);

  }
}

void handleStrike() {
  Serial.println("Mallet strike!");
  ledIsOn = true;
  lastStrikeTime = loopTime;
  digitalWrite(kStrikeLedPin, HIGH);
}

void turnOffStrikeLed() {
  if(ledIsOn && loopTime > lastStrikeTime + kLedHoldTime) {
    Serial.println("Turning off led");
    ledIsOn = false;
    digitalWrite(kStrikeLedPin, LOW);
  }
}

void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  imu.readGyro();

  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  Serial.print("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  Serial.print(imu.calcGyro(imu.gx), 2);
  Serial.print(", ");
  Serial.print(imu.calcGyro(imu.gy), 2);
  Serial.print(", ");
  Serial.print(imu.calcGyro(imu.gz), 2);
  Serial.println(" deg/s");
#elif defined PRINT_RAW
  Serial.print(imu.gx);
  Serial.print(", ");
  Serial.print(imu.gy);
  Serial.print(", ");
  Serial.println(imu.gz);
#endif
}

void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  imu.readAccel();

  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  Serial.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.az), 2);
  Serial.println(" g");
#elif defined PRINT_RAW
  Serial.print(imu.ax);
  Serial.print(", ");
  Serial.print(imu.ay);
  Serial.print(", ");
  Serial.println(imu.az);
#endif

}

void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  imu.readMag();

  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  Serial.print("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  Serial.print(imu.calcMag(imu.mx), 2);
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.my), 2);
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.mz), 2);
  Serial.println(" gauss");
#elif defined PRINT_RAW
  Serial.print(imu.mx);
  Serial.print(", ");
  Serial.print(imu.my);
  Serial.print(", ");
  Serial.println(imu.mz);
#endif
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(
float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);

  heading -= kDeclination * M_PI / 180;

  if (heading > M_PI) heading -= (2 * M_PI);
  else if (heading < -M_PI) heading += (2 * M_PI);
  else if (heading < 0) heading += 2 * M_PI;

  // Convert everything from radians to degrees:
  heading *= 180.0 / M_PI;
  pitch *= 180.0 / M_PI;
  roll  *= 180.0 / M_PI;

  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Heading: "); Serial.println(heading, 2);
}
