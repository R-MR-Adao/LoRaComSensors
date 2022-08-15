#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>    // for BME press/hum sensor
#include <Adafruit_BME280.h>    // for BME press/hum sensor

// BME280 definitions
typedef Adafruit_BME280    BME;    // use abbreviations instead of full names 
BME   bme; // 0x76 i2C address
// end of BME280 definitions

void setup()
{
  // initialize Serial
  Serial.begin(9600);
  // initialize BME280
  unsigned status;
  status = bme.begin();
  while (!status)
  { // keeps trying to initialize the BME280
    status = bme.begin();
    printLastOperateStatus_BME280(bme.sensorID());
    delay(2000);
  }
  Serial.println("BME280 begin success");
  // end of BME initialize
}
void loop() 
{
  // read data from sensors
  // data from BME 280
  float     hum = bme.readHumidity(); // (%)
  float     temp2 = bme.readTemperature(); // (%)
  // display data on serial
  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print("   Rh (%): "); Serial.print(hum);
  Serial.print("   T2 (oC): "); Serial.println(temp2);
  Serial.print("   NO2 (ppb): "); Serial.println(NO2_Concentration);
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}
void printLastOperateStatus_BME280(int sensorID) // BME280
{
  Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  Serial.print("SensorID was: 0x"); Serial.println(sensorID, 16);
  Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
  Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
  Serial.print("        ID of 0x60 represents a BME 280.\n");
  Serial.print("        ID of 0x61 represents a BME 680.\n");
}