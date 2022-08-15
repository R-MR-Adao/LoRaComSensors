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
void setup()
{
    // initialize Serial
  Serial.begin(9600);
  // initialize DGS-NO2
  Serial2.begin(9600,SERIAL_8N1);  // serial ports 16 and 17 for communication with digital board
  Serial2.setTimeout(1000);
  flush_Serial2();
  flush_Serial2();
  // end of initialize DGS-NO2
}
void loop()
{
  // read data from sensors
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
  Serial.print("   NO2 (ppb): "); Serial.println(NO2_ConcentrationAvg);
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
  //Serial.println(NO2_dataString);
}
void SPEC_parse_data(){
  // Parses the received NO2_dataString
  // Data string is comma separated
  // The format of the output is:  The format of the output is:
  //SN, PPB, T (oC), RH (%), ADC Raw, T Raw, RH Raw, Day, Hour, Minute, Second 
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