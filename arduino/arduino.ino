#include <SoftwareSerial.h>
SoftwareSerial esp8266(6, 7); //RX,TX
int responseTime[3];
int secondPin = 5;
void setup()
{
  String response = "";
  pinMode(secondPin, OUTPUT);
  Serial.begin(9600);
  esp8266.begin(9600);

  getResponse("ATE0\r\n", 1000, "ECHO OFF");

  getResponse("AT+CIPSNTPCFG=1,-4\r\n", 1000, "SETTING TIME ZONE");

  
  response = getResponse("AT+CIPSNTPTIME?\r\n", 5000, "DISPLAYING TIME ATTEMPT 1");
  getTime(response);

  response = getResponse("AT+CIPSNTPTIME?\r\n", 5000, "DISPLAYING TIME ATTEMPT 2");
  getTime(response);

  response = getResponse("AT+CIPSNTPTIME?\r\n", 5000, "DISPLAYING TIME ATTEMPT 3");
  getTime(response);

}

void loop() {
  if(responseTime[2] != -1) {
   analogWrite(secondPin, responseTime[2] * 4.25);
   delay(1000);
   responseTime[2] += 1;
   Serial.print(responseTime[2]);
   Serial.print(" , ");
   Serial.print(responseTime[2] * 4.25);
   Serial.print("\n");
   if(responseTime[2] == 59) { 
    responseTime[2] = 0;
   }
  }
}



String getResponse(String atCommand, int del, String message) {
  esp8266.print(atCommand);
  delay(del);
  Serial.print("********************* ");
  Serial.print(message);
  Serial.print(" ********************\n");
  String response = "";
  while (esp8266.available())     {
    char c = esp8266.read();
    response += c;
    Serial.print(c);
  }
  return response;
}

void getTime(String response) {
  int year = response.substring(33, 37).toInt();
  Serial.println(year == 1970 ? "EMPTY -- SKIPPING" : "PARSING: '" + response + "'");
  if (year != 1970) {
    responseTime[0] = getHour(response);
    Serial.println("HOUR: " + String(responseTime[0]));
    responseTime[1] = getMinute(response);
    Serial.println("MINUTE: " + String(responseTime[1]));    
    responseTime[2] = getSecond(response);
    Serial.println("SECOND: " + String(responseTime[2]));
  }
}
int getHour(String response) {
  return response.substring(24, 26).toInt();
}
int getMinute(String response) {
  return response.substring(27, 29).toInt();
}
int getSecond(String response) {
  return response.substring(28, 30).toInt();
}
//+CIPSNTPTIME:Thu Jan 01 00:00:00 1970
