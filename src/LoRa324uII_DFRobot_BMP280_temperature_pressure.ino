#include <Wire.h>
#include <SPI.h>
#include "DFRobot_BMP280.h"     // for BMP temp/press sensor

// BMP 280 definitions
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);
typedef DFRobot_BMP280_IIC    BMP;    // use abbreviations instead of full names
BMP   bmp(&I2Cone, BMP::eSdo_high); // 0x77 i2C address
#define SEA_LEVEL_PRESSURE    1025.0f   // sea level pressure in Vigo
// end of BMP 280 definitions

void setup()
{
    // initialize Serial
  Serial.begin(9600);
  // initialize bmp280
  bmp.reset();
  Serial.println("bmp read data test");
  while (bmp.begin() != BMP::eStatusOK)
  { // keeps trying to initialize the bmp280
    Serial.println("bmp begin faild");
    printLastOperateStatus_BMP280(bmp.lastOperateStatus);
    delay(2000);
  }
  Serial.println("BMP280 begin success");
  // end of bmp280 initialize
}
void loop()
{
    // read data from sensors
  // data from BMP 280
  float     temp = bmp.getTemperature(); // (oC)
  uint32_t  pres = bmp.getPressure();  // (Pa)
  float     alti = 44330 * (1.00000f - pow((float) pres / 100 / SEA_LEVEL_PRESSURE, 0.1903)); // (m)
  // display data on serial
  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print("T (oC): "); Serial.print(temp);
  Serial.print("   P (Pa): "); Serial.print(pres);
  Serial.print("   h (m): "); Serial.println(alti);
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}
void printLastOperateStatus_BMP280(BMP::eStatus_t eStatus) // for bmp280
{ // function used in the setup in case the sensor fails to initialize
  switch (eStatus) {
    case BMP::eStatusOK:    Serial.println("everything ok"); break;
    case BMP::eStatusErr:   Serial.println("unknow error"); break;
    case BMP::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
    case BMP::eStatusErrParameter:    Serial.println("parameter error"); break;
    default: Serial.println("unknow status"); break;
  }
}