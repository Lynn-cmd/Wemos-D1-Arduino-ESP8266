#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <AccelStepper.h>
#ifndef STASSID
#define STASSID "CU_W"
#define STAPSK "Aa123456"
#endif
// // X slot
// const int xdirPin = 14;     // D5方向控制引脚
// const int xstepPin = 16;    // D2步进控制引脚
// // Y slot
// const int xdirPin = 12;     // D6方向控制引脚
// const int xstepPin = 5;    // D3步进控制引脚
// // Z slot
// const int xdirPin = 13;     // D7方向控制引脚
// const int xstepPin = 4;    // D4步进控制引脚
// A slot
const int xdirPin = 14;    // D13方向控制引脚
const int xstepPin = 12;   // D12步进控制引脚

const int xenablePin = 0;  // D8使能控制引脚
const int moveSteps = 200;    //运行步数
const int led = 2;

AccelStepper stepper1 = AccelStepper(1,xstepPin,xdirPin);
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);
float maxspeed = 1500.0;
float rspeed = 700.0;
bool running = false;

void handleSpeedChange() {
   String message = "";
   String spd = "";
   float speed = 0;
  //  float rspeed = 0;
    if (server.arg("spd") != "")//Parameter 'spd'
    {     
      // // message = "Speed Argument not found";
      // }else{     //Parameter found
      message = "Speed=" ;
      message += server.arg("spd");//Gets the value of the query parameter
      spd = server.arg("spd");
      speed = spd.toFloat();
      }
  Serial.println(message);
  rspeed = ( speed / 100.0 ) * 1000.0;
  message = rspeed;
  Serial.println("Real time speed = ");
  Serial.println(message);
  server.send(200, "text/plain", "Speed changed");  //Returns the HTTP response
 }

 void handle1() {
  for(int i=0; i < 1; i++){
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    server.send(200, "text/plain", "Blink LED 1");  //Returns the HTTP response
  }
  digitalWrite(led, LOW);
}

void handle2() {
for(int i=0;i < 2; i++){
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    server.send(200, "text/plain", "Blink LED 2");  //Returns the HTTP response
  }
  digitalWrite(led, LOW);
}

 void handle5() {
   running = !running;
   if (rspeed < 0) { rspeed = 0 - rspeed; }
  server.send(200, "text/plain", "Set run as clockwise"); }

void handle4() {
 running = !running;
  if (rspeed > 0) { rspeed = 0 - rspeed; }
  server.send(200, "text/plain", "Set run as anti-clockwise"); }
 
void handleRoot() { server.send(200, "text/plain", "Root path of esp8266!\r\n");}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);// Wait for connection
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(xstepPin,OUTPUT);     // Arduino控制A4988步进引脚为输出模式
  pinMode(xdirPin,OUTPUT);      // Arduino控制A4988方向引脚为输出模式
  // digitalWrite(xdirPin,HIGH);  
  pinMode(xenablePin,OUTPUT);   // Arduino控制A4988使能引脚为输出模式
  digitalWrite(xenablePin,LOW); // 将使能控制引脚设置为低电平从而让电机驱动板进入工作状态
  // running = false;
  stepper1.setMaxSpeed(maxspeed);     // 设置电机最大速度300 
  stepper1.setAcceleration(100.0);  // 设置电机加速度20.0  
  rspeed = 900.0;//Initial speed

  server.on("/", handleRoot);
  server.on("/1", handle1);
  server.on("/2", handle2);
  server.on("/4", handle4);
  server.on("/5", handle5);
  server.on("/speed", handleSpeedChange);

  server.begin();
  Serial.println("HTTP server started");

  // digitalWrite(led,HIGH); 
  // delay(1000);           
  // digitalWrite(led,LOW); 
  // delay(1000);
}

void loop(void) {
  server.handleClient();
  if( running )
 {
   stepper1.stop();
 }
  else
  {
  stepper1.enableOutputs();
  stepper1.setCurrentPosition(0);
  Serial.println(rspeed);
  stepper1.setSpeed(rspeed);
  stepper1.runSpeed();
  }
}
