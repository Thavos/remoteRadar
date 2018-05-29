#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <Servo.h>

#define TRIGGER 0         //TRIG PIN sensor D3
#define ECHO    5        //ECHO PIN sensor  D1
#define TRIGGER2 12         //TRIG PIN sensor D6
#define ECHO2    13         //ECHO PIN sensor D7
#define NUM_LEDS 12 //How many leds on the strip?
#define DATA_PIN 16
#define SRVO 14             //D4
Servo servo;
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
CRGB leds[NUM_LEDS];
HTTPClient http;
char JSONmessageBuffer[300];
long duration, distance,duration2,distance2;
int pozicia=0,strana=1,zelena,zelena2;
void setup() {
  
  servo.attach(SRVO);
  
  Serial.begin (9600);
  const char* ssid = "GL-EDU";
  const char* password = "gl.edu.123!";
  WiFi.begin(ssid, password);                           //connecting to wifi
 
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(1000);
    Serial.println("Connecting..");
    
  }
  
  pinMode(TRIGGER, OUTPUT);                             //setup for ultrasonic s.
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER2, OUTPUT);                             //setup for ultrasonic s.
  pinMode(ECHO2, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(48);
}

void loop() {
  servo.write(pozicia/1.27);
  
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2);
  Serial.print(distance);
  Serial.print("    ");
  delay(15);
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  distance2 = (duration2/2) / 29.1;
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
  Serial.print(distance2);
  Serial.print("    ");
  delay(10);

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
  /*
  if(distance>199)
  {
    distance=199;
  }
  if(distance2>199)
  {
    distance2=199;
  }
  if(distance<3)
  {
    distance=3;
  }
  if(distance2<3)
  {
    distance2=3;
  }
  zelena=map(distance,3,200,0,255);
  zelena2=map(distance2,3,200,0,255);
  if(pozicia<30)
  {
    leds[1]= CRGB(255-zelena,zelena,0);
    
    leds[7]= CRGB(255-zelena2,zelena2,0);
  }
  else if(pozicia>=30&&pozicia<60)
  {
    leds[2].g=zelena;
    leds[2].r=255-zelena;
    leds[8].g=zelena2;
    leds[8].r=255-zelena2;
  }
  else if(pozicia>=60&&pozicia<90)
  {
    leds[3].g=zelena;
    leds[3].r=255-zelena;
    leds[9].g=zelena2;
    leds[9].r=255-zelena2;
  }
  else if(pozicia>=90&&pozicia<120)
  {
    leds[4].g=zelena;
    leds[4].r=255-zelena;
    leds[10].g=zelena2;
    leds[10].r=255-zelena2;
  }
  else if(pozicia>=120&&pozicia<150)
  {
    leds[5].g=zelena;
    leds[5].r=255-zelena;
    leds[11].g=zelena2;
    leds[11].r=255-zelena2;
  }
  else if(pozicia>=150)
  {
    leds[6].g=zelena;
    leds[6].r=255-zelena;
    leds[12].g=zelena2;
    leds[12].r=255-zelena2;
  }
  FastLED.show();
  */
  JSONencoder["id"] = "2";
  JSONencoder["angle"] = pozicia;
  JSONencoder["angleunit"] = "degree";
  JSONencoder["distance"] = distance;
  JSONencoder["distanceUnit"] = "mm";
  JSONencoder["distanceBack"] = distance2;
  
  

  
  
  HTTPClient http; //Declare object of class HTTPClient
  
  http.begin("http://172.17.116.241:8888/api/v1"); //Specify request destination

  http.addHeader("Content-Type", "application/json"); //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request

  String payload = http.getString(); //Get the response payload

  Serial.println(httpCode); //Print HTTP return code

  Serial.println(payload); //Print request response payload

  http.end(); //Close connection
  
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));


  Serial.println(JSONmessageBuffer);
}
