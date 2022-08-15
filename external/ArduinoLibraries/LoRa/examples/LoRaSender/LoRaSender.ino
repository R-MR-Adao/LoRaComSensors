#include <SPI.h>
#include <LoRa.h>

// uncomment the section corresponding to your board
// BSFrance 2017 contact@bsrance.fr 

//  //LoR32u4 433MHz V1.0 (white board)
//  #define SCK     15
//  #define MISO    14
//  #define MOSI    16
//  #define SS      1
//  #define RST     4
//  #define DI0     7
//  #define BAND    433E6 
//  #define PABOOST true

//  //LoR32u4 433MHz V1.2 (white board)
//  #define SCK     15
//  #define MISO    14
//  #define MOSI    16
//  #define SS      8
//  #define RST     4
//  #define DI0     7
//  #define BAND    433E6 
//  #define PABOOST true 

  //LoR32u4II 868MHz or 915MHz (black board)
  #define SCK     15
  #define MISO    14
  #define MOSI    16
  #define SS      8
  #define RST     4
  #define DI0     7
  #define BAND    868E6  // 915E6
  #define PABOOST true 

  #define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
  long tem,hum;
  int count=0;
  int device_id=12345; // ID of this End node
  int counter = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSignalBandwidth(125E3);
  LoRa.setFrequency(868300000);
  LoRa.setSpreadingFactor(7);
  LoRa.enableCrc();
  LoRa.setSyncWord(52);
  LoRa.setPreambleLength(8);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  tem = random(25,35);   // Generate a random temperature.
  hum = random(85,95);   // Generate a random humidity.

  // compose and send packet
  LoRa.beginPacket();
  // LoRa.print("<12345>field1=12&field2=34");
  LoRa.print("<");
  LoRa.print(device_id);
  LoRa.print(">temp=");
  LoRa.print(tem);
  LoRa.print("&hum=");
  LoRa.print(hum); 
   LoRa.print("&count="); 
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 

  
//  Serial.print("Sending packet: ");
//  Serial.println(counter);
//  // send packet
//  LoRa.beginPacket();
//  LoRa.print("hello ");
//  LoRa.print(counter);
//  LoRa.endPacket();
//  counter++;
//  delay(5000);
}
