//#include <LoRa.h>
#include <Wire.h>
#include <SPI.h>

#include "heltec.h"             // for esp32 board
#include "DFRobot_BMP280.h"     // for BMP temp/press sensor
#include <Adafruit_Sensor.h>    // for BME press/hum sensor
#include <Adafruit_BME280.h>    // for BME press/hum sensor
#include <dummy.h>              // for esp32 board

// LoRa communication definitions
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
long tem, hum;
int count = 0;
int device_id = 00002; // ID of this End node
int counter = 0;
// end of communication definitions

// BMP 280 definitions
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);
typedef DFRobot_BMP280_IIC    BMP;    // use abbreviations instead of full names
BMP   bmp(&I2Cone, BMP::eSdo_high); // 0x77 i2C address
#define SEA_LEVEL_PRESSURE    1025.0f   // sea level pressure in Vigo
// end of BMP 280 definitions

// BME280 definitions
typedef Adafruit_BME280    BME;    // use abbreviations instead of full names 
BME   bme; // 0x76 i2C address
// end of BME280 definitions

// Analog Sound Sensor v2.2 definitions
int SoundAnalogPin = 2; // analog pin where the sound sensor is connected
float nAverageTimes = 25; // number of times the sound value is integrated per measurements
//end of Analog Sound Sensor v2.2 definitions

// DGS-NO2 definitions
String NO2_SensorSerialNo; 
int NO2_averageN = 1;
int NO2_Concentration;
int NO2_Temperature;
int NO2_RH;
int NO2_RawSensor;
int NO2_TempDigital;
int NO2_RHDigital;
int NO2_Days;
int NO2_Hours;
int NO2_Minutes;
int NO2_Seconds;
#define NO2_command_delay 600
#define NO2_start_delay 2500
String NO2_dataString = "";
String NO2_responseString = "";
boolean NO2_dataStringComplete = 0;
char NO2_inChar;

// end of DGS-NO2 definitions

void setup() {
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

  // initialize DGS-NO2
  Serial2.begin(9600,SERIAL_8N1);  // serial ports 16 and 17 for communication with digital board
  Serial2.setTimeout(1000);
  flush_Serial2();
  flush_Serial2();
  // end of initialize DGS-NO2

  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, false /*Serial Enable*/, false /*PABOOST Enable*/, BAND /*long BAND*/);
  
  // initialize LoRa
  LoRa.setSignalBandwidth(125E3);
  LoRa.setFrequency(868300000);
  LoRa.setSpreadingFactor(7);
  LoRa.enableCrc();
  LoRa.setSyncWord(52);
  LoRa.setPreambleLength(8);
  // end of initialize LoRa

  delay(100);
}

void loop() {
  // read data from sensors
  // data from BMP 280
  float     temp = bmp.getTemperature(); // (ºC)
  uint32_t  pres = bmp.getPressure();  // (Pa)
  float     alti = 44330 * (1.00000f - pow((float) pres / 100 / SEA_LEVEL_PRESSURE, 0.1903)); // (m)
  // data from BME 280
  float     hum = bme.readHumidity(); // (%)
  float     temp2 = bme.readTemperature(); // (%)
  // data from Analog Sound sensor v2.2
  float     snd = 0;
  for(int i = 0;i < nAverageTimes; i++) // average the measure sound intensity 25 times
  {
     snd = snd + analogRead(SoundAnalogPin);
     delay(50); // stabilization time
  }
  snd = snd / nAverageTimes; // analog value 0 - 2^12
  snd = 50*(1 -exp(-(snd+1200)/500)) + 21./(1 + exp(-(snd-3600)/360)); // use calibration to convert to dBA
  // data from DGS-NO2
  Serial2.print(" ");         // Now we trigger a measurement
  delay(NO2_command_delay);   // wait for sensor to respond
  Serial2.print("c");         // start continuous reading
  int NO2_ConcentrationAvg = 0;
  for(int i = 0; i < NO2_averageN; i++) // average the measured value N times
  {
    SPEC_Data_read();
    SPEC_parse_data(); 
    NO2_ConcentrationAvg += NO2_Concentration;
  }
  Serial2.print("c"); // stop continuous reading
  Serial2.print("s"); // standby
  NO2_ConcentrationAvg = (float) NO2_ConcentrationAvg / NO2_averageN;

  // display data on serial
  Serial.print("Sending packet: ");
  Serial.println(counter);
  //Serial.println("======== start print ========");
  Serial.print("T (oC): "); Serial.print(temp);
  Serial.print("   P (Pa): "); Serial.print(pres);
  Serial.print("   h (m): "); Serial.println(alti);
  Serial.print("   Rh (%): "); Serial.print(hum);
  Serial.print("   T2 (ºC): "); Serial.println(temp2);
  Serial.print("   NO2 (ppb): "); Serial.println(NO2_ConcentrationAvg);
  //Serial.println("========  end print  ========");


  // compose and send packet
  LoRa.beginPacket();
  // format: "<device_id>temp=TTT&press=PPP&alt=AAA&count=CCC"
  LoRa.print("<");
  LoRa.print(device_id);

  LoRa.print(">temp=");
  LoRa.print(temp);

  LoRa.print("&pres=");
  LoRa.print(pres);

  LoRa.print("&alt=");
  LoRa.print(alti); // this should be adjusted to your local forcase

  LoRa.print("&hum=");
  LoRa.print(hum);

  LoRa.print("&NO2=");
  LoRa.print(NO2_Concentration);

  LoRa.print("&snd=");
  LoRa.print(snd);

  LoRa.print("&count=");
  LoRa.print(counter);

  LoRa.endPacket();

  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
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

void printLastOperateStatus_BME280(int sensorID) // BME280
{
  Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  Serial.print("SensorID was: 0x"); Serial.println(sensorID, 16);
  Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
  Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
  Serial.print("        ID of 0x60 represents a BME 280.\n");
  Serial.print("        ID of 0x61 represents a BME 680.\n");
}

void flush_Serial2(){
  // Do we have data in the serial buffer?
  // If so, flush it
  Serial.println("Flushing Serial2");
  if (Serial2.available() > 0){
    Serial.println ("Flushing serial buffer...");
    while(1){
      NO2_inChar = (char)Serial2.read();
      delay(10);
      Serial.print(NO2_inChar);
      if (NO2_inChar == '\n') break; 
    }
    Serial.println (" ");
    Serial.println ("Buffer flushed!");
  }
  else
  {
    Serial.println("Nothing to flush!");
  }
}

void SPEC_dump_EEPROM(){
  Serial.println("Dumping EEPROM");
  // First we trigger a measurement
  Serial2.print(" ");
  // Within one second time we send the command "e"
  delay(NO2_start_delay);
  Serial2.print("e");
  NO2_dataString = Serial2.readStringUntil('\n');
  // You can uncomment this line if you wish
  Serial.println(NO2_dataString);
  for (int i=0; i<20; i++){ 
    NO2_responseString = Serial2.readStringUntil('\n');
    if(i <= 19)
    {
      Serial.print("Response string ");
      Serial.print(i);
      Serial.print(": ");
    }
      
    Serial.println(NO2_responseString);
  }   
}  

void SPEC_Data_read(){
  // First, we do some initialization
  // NO2_dataStringComplete is set as "false", as we don't have any valid data received
  NO2_dataStringComplete = 0;
  // Clear the data string
  NO2_dataString = "";
  NO2_dataString = Serial2.readStringUntil('\n');
  // sometimes the string is read as empty... try to read it 3 times
  int i = 0;
  while ((NO2_dataString.length() == 0) && (i < 2))
  { 
    NO2_dataString = Serial2.readStringUntil('\n');
    i = i + 1;
  }
  Serial.print("NO2_dataString: ");
}


void SPEC_parse_data(){
  // Parses the received NO2_dataString
  // Data string is comma separated
  // The format of the output is:  The format of the output is:
  //SN, PPB, T (°C), RH (%), ADC Raw, T Raw, RH Raw, Day, Hour, Minute, Second 
  //SN[XXXXXXXXXXXX], PPB [0 : 999999], TEMP [-99:99],
  // RH[0:99], RawSensor[ADCCount], TempDigital, RHDigital, Day[0:99], Hour [0:23],
  // Minute[0:59], Second[0 : 59]\r\n
  if (NO2_dataString.length() > 0)
  {
    int idx1 = NO2_dataString.indexOf(',');
    NO2_SensorSerialNo = NO2_dataString.substring(0, idx1);
    int idx2 = NO2_dataString.indexOf(',', idx1 + 1);
    // Hint: after comma there's a space - it should be ignored
    String S_gas = NO2_dataString.substring(idx1 + 2, idx2);
    NO2_Concentration = S_gas.toInt();
    int idx3 = NO2_dataString.indexOf(',', idx2 + 1);
    String S_temp = NO2_dataString.substring(idx2 + 2, idx3);
    NO2_Temperature = S_temp.toInt();
    int idx4 = NO2_dataString.indexOf(',', idx3 + 1);
    String S_humi = NO2_dataString.substring(idx3 + 2, idx4);
    NO2_RH = S_humi.toInt();
    int idx5 = NO2_dataString.indexOf(',', idx4 + 1);
    String S_raw_gas = NO2_dataString.substring(idx4 + 2, idx5);
    NO2_RawSensor = S_raw_gas.toInt();
    int idx6 = NO2_dataString.indexOf(',', idx5 + 1);
    String S_Tdigital = NO2_dataString.substring(idx5 + 2, idx6);
    NO2_TempDigital = S_Tdigital.toInt();
    int idx7 = NO2_dataString.indexOf(',', idx6 + 1);
    String S_RHdigital = NO2_dataString.substring(idx6 + 2, idx7);
    NO2_RHDigital = S_RHdigital.toInt();
    int idx8 = NO2_dataString.indexOf(',', idx7 + 1);
    String S_Days = NO2_dataString.substring(idx7 + 2, idx8);
    NO2_Days = S_Days.toInt();
    int idx9 = NO2_dataString.indexOf(',', idx8 + 1);
    String S_Hours = NO2_dataString.substring(idx8 + 2, idx9);
    NO2_Hours = S_Hours.toInt();
    int idx10 = NO2_dataString.indexOf(',', idx9 + 1);
    String S_Minutes = NO2_dataString.substring(idx9 + 2, idx10);
    NO2_Minutes = S_Minutes.toInt();
    int idx11 = NO2_dataString.indexOf('\r');
    String S_Seconds = NO2_dataString.substring(idx10 + 2, idx11);
    NO2_Seconds = S_Seconds.toInt();
  }
  
}
