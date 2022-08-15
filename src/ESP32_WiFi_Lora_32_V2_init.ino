#include "heltec.h"             // for esp32 board
#include <dummy.h>              // for esp32 board

// LoRa communication definitions
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
int count = 0;
int device_id = 00002; // ID of this End node
int counter = 0;
// end of communication definitions

void setup()
{
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
void loop()
{
  // compose and send packet
  LoRa.beginPacket();
  // format: "<device_id>meas1=111&meas2=222&meas3=333&count=CCC"
  LoRa.print("<");
  LoRa.print(device_id);
  LoRa.print("&count=");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}