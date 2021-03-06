#include <Servo.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define NUM_LEDS 12 //How many leds on the strip?
#define DATA_PIN 4  //DIN of LED strip - D4
#define TRIGGER1 D5 // Trigger of main ultrasonic sensor
#define ECHO1    D7
#define TRIGGER2 D6
#define ECHO2    D8
//Servopin : D3

int angle; //Actual angle of servo
int angle2; //Opposite angle (of second sensor)
int anglemovement=0;
int ledid;
int dist1, dist2;
int mnumber;
int ledarray [2][6] = {{9, 8, 7, 6, 5, 4},
                       {3, 2, 1, 0, 11, 10}};
bool rot = 0; //Rotation direction; 0=CCW; 1=CW
long duration1;
long duration2;
long distance1;
long distance2;

Servo servo;
CRGB leds[NUM_LEDS];
                            
void setup() {
      Serial.begin(9600);
  //Wifi
       delay(5000);
      const char* ssid = ""; //SSID of WiFi router
      const char* password = ""; //PASS for router
      WiFi.begin(ssid, password); //Connect to wifi
      while (WiFi.status() != WL_CONNECTED) {
              delay(500);
              Serial.println("Awaiting connection"); //As long as not connected
      }
      Serial.println("");
      Serial.println("WiFi connected"); 
      Serial.println(WiFi.localIP()); //Print the IP address
  //Servo            
      angle = 90; //set home position
      servo.attach(D3); //Servo pin
      servo.write(0); //Bring servo to home position
  //LED
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      FastLED.clear();
      FastLED.setBrightness(48);
  //Ultrasonic sensor
      pinMode(TRIGGER1, OUTPUT);
      pinMode(ECHO1, INPUT);
      pinMode(TRIGGER2, OUTPUT);
      pinMode(ECHO2, INPUT);
}

void loop(){
  for(int timer = 0; timer<30; timer=timer+5){
    movement();
    //measured();
    measure1();
    measure2();
    mprint();
    jsonvoid();
    delay(500);
    //Serial.println(WiFi.localIP()); //Print the IP address
    //Serial.println();
  }
  ledvoid();
}

void movement(){
   if(angle>174){
        rot=0;
   }
   else if(angle<1){
   rot=1;           //Servo reverse point
   }
   if(rot==1){
       angle = angle+5;
       anglemovement = ((angle+25)*150)/180;
    }
   else{
       angle = angle-5;
       anglemovement = ((angle+25)*150)/180;
   }
    
  //Serial.println(anglemovement);
    servo.write(anglemovement);       //Servo move by 5°
    angle2 = angle+180;
}

void measured(){
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  distance2 = (duration2/2) / 29.1;
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
  delay(150);
}

void measure1(){
      //Measure the distances
  digitalWrite(TRIGGER1, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIGGER1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER1, LOW);
  duration1 = pulseIn(ECHO1, HIGH);
  digitalWrite(TRIGGER1, LOW);  
  delayMicroseconds(2);
      //Change the distances to milimeters
  distance1 = (duration1/2) / 2.91;
  delay(150);
}

void measure2(){
      //Measure the distances
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2);
      //Change the distances to milimeters
  distance2 = (duration2/2) / 2.91;
  delay(150);
}

void mprint(){
      //convert to milimetres
  Serial.print("The distance at ");
  Serial.print(angle);
  Serial.print("° is ");
  Serial.println(distance1);
  Serial.print("The distance at ");
  Serial.print(angle2);
  Serial.print("° is ");
  Serial.println(distance2);
  Serial.println("-----------------------------");
  }

void ledvoid(){
  dist1=distance1;
  dist2=distance2;
  if(distance1>1000){
    dist1=999;}
  if(distance2>1000){
    dist2=999;}
  
  ledid=angle/30; 
  int greenval1 = map(dist1, 30, 1000, 0, 255); 
  int greenval2 = map(dist2, 30, 1000, 0, 255); 
 
  leds[ledarray[0][ledid]].g = greenval1;
  leds[ledarray[0][ledid]].r = 255-greenval1;
  leds[ledarray[1][ledid]].g = greenval2;
  leds[ledarray[1][ledid]].r = 255-greenval2;
  FastLED.show();
  delay(100);
  //Serial.println(ledid);
}

void jsonvoid(){
  StaticJsonBuffer<300> JSONbuffer;
  /*if(mnumber==37)
        {mnumber=1;}*/
  mnumber = angle/5;
/*  if (angle = 180)
      {mnumber = 1;}*/
  mnumber = mnumber+1;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["id"] = mnumber;
  //JSONencoder["angleFront"] = angle;
  JSONencoder["distanceFront"] = distance1;
  //JSONencoder["angleBack"] = angle2;
  JSONencoder["distanceBack"] = distance2;
  char JSONmessageBuffer[300];

  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  HTTPClient http; //Declare object of class HTTPClient
  
  http.begin("http://192.168.10.59:8080/arduino/data"); //Specify request destination
  http.addHeader("Content-Type", "application/json"); //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request
  String payload = http.getString(); //Get the response payload

  /*Serial.println(httpCode); //Print HTTP return code
  Serial.println(payload); //Print request response payload*/
  Serial.println(JSONmessageBuffer);
  http.end(); //Close connection
  //mnumber++;
}

