// Analog Sound Sensor v2.2 definitions
int SoundAnalogPin = 2; // analog pin where the sound sensor is connected
float nAverageTimes = 25; // number of times the sound value is integrated per measurements
//end of Analog Sound Sensor v2.2 definitions
void setup()
{
  // initialize Serial
  Serial.begin(9600);
}
void loop()
{
  // read data from sensors
  // data from Analog Sound sensor v2.2
  float     snd = 0;
  for(int i = 0;i < nAverageTimes; i++) // average the measure sound intensity 25 times
  {
     snd = snd + analogRead(SoundAnalogPin);
     delay(50); // stabilization time
  }
  snd = snd / nAverageTimes; // analog value 0 - 2^12
  // use calibration to convert to dBA
  snd = 50*(1 -exp(-(snd+1200)/500)) + 21./(1 + exp(-(snd-3600)/360));
  // display data on serial
  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print("   SI (dBA): "); Serial.println(snd);
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}