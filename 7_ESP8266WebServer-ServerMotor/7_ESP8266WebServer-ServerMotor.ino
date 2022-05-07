#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
// #include <AccelStepper.h>
#ifndef STASSID
#define STASSID "CU_W"
#define STAPSK "Aa123456"
#endif
const int xdirPin = 14;     // D5方向控制引脚
const int xstepPin = 16;    // D2步进控制引脚
const int xenablePin = 0;  // D8使能控制引脚
const int moveSteps = 200;    //运行步数
const int LED = 2;
bool running = false;
//  AccelStepper stepper1 = AccelStepper(1,xstepPin,xdirPin);//建立步进电机对象

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);

 void handle1() {
  running = !running;
  digitalWrite(xdirPin, HIGH);
  server.send(200, "text/plain", "Msg4 from esp8266!\r\n");} 
 
void handle2() { 
  running = !running;
 digitalWrite(xdirPin, LOW);
 server.send(200, "text/plain", "Msg5 from esp8266!\r\n");}
 
void handleled() {
  digitalWrite(LED,HIGH);   
  delay(1000);              
  digitalWrite(LED,LOW);    
  delay(1000);              
  server.send(200, "text/plain", "LED blink from esp8266!\r\n");}

void handleRoot() { server.send(200, "text/plain", "hello from esp8266!\r\n");}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(xstepPin,OUTPUT);     // Arduino控制A4988步进引脚为输出模式
  pinMode(xdirPin,OUTPUT);      // Arduino控制A4988方向引脚为输出模式
  digitalWrite(xdirPin,HIGH);  
  pinMode(xenablePin,OUTPUT);   // Arduino控制A4988使能引脚为输出模式
  digitalWrite(xenablePin,LOW); // 将使能控制引脚设置为低电平从而让电机驱动板进入工作状态
  // running = false;

  server.on("/", handleRoot);
  server.on("/1", handleled);
  server.on("/4", handle1);
  server.on("/5", handle2);
  
  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(LED,HIGH); 
  delay(1000);           
  digitalWrite(LED,LOW); 
  delay(1000);
}

void loop(void) {
  server.handleClient();
  if(running)
      {digitalWrite(xstepPin,LOW);}
    else
    {
        digitalWrite(xstepPin,HIGH); // Output high
        delayMicroseconds(400); // Wait 1/2 a ms  340--1500
        digitalWrite(xstepPin,LOW); // Output low
        delayMicroseconds(400); // Wait 1/2 a ms 340--1500
    }
}
