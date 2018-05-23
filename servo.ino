#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#define TRIGGER 5         //TRIG PIN sensor
#define ECHO    4         //ECHO PIN sensor
Servo servo;

int pozicia=0,strana=1;
void setup() {
  Serial.begin (9600);
  servo.attach(D4);
  const char* ssid = "id";
  const char* password = "password";
  WiFi.begin(ssid, password);                           //connecting to wifi
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(1000);
    Serial.println("Connecting..");
  }
  pinMode(TRIGGER, OUTPUT);                             //setup for ultrasonic s.
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  servo.write(pozicia);

  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2);
  
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.print("distance = ");
  Serial.print(distance);
  Serial.print("      angle = ");
  Serial.println(pozicia);

  StaticJsonBuffer<300> JSONbuffer;

  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["distance"] = distance;
  char JSONmessageBuffer[300];

  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  HTTPClient http; //Declare object of class HTTPClient
  
  http.begin("http://anteph.pythonanywhere.com/postjson"); //Specify request destination

  http.addHeader("Content-Type", "application/json"); //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request

  String payload = http.getString(); //Get the response payload

  Serial.println(httpCode); //Print HTTP return code

  Serial.println(payload); //Print request response payload

  http.end(); //Close connection
  
  


  Serial.println(JSONmessageBuffer);
  if(strana==1)                                          //servo movement
  {
      pozicia=pozicia+5;
  }
  else
  {
      pozicia=pozicia-5;
  }
  if(pozicia>174)
      strana=0;
  else if(pozicia<5)
      strana=1;
  delay(500);
}



