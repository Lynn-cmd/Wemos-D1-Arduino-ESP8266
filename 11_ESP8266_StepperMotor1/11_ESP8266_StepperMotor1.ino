/*simulate RC control panel
123     ↖  ↑  ↗
456 =   ←  ㊣  →
789     ↙  ↓  ↘
L = turn left; R = turn right */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <AccelStepper.h>
#ifndef STASSID
#define STASSID "CU_W"
#define STAPSK "Aa123456"
#define motorInterfaceType 1
#endif
// 定义电机控制用常量
 
// A4988连接Arduino引脚号
const int xdirPin = 5;     // 方向控制引脚
const int xstepPin = 2;    // 步进控制引脚
const int xenablePin = 8;  // 使能控制引脚
const int moveSteps = 200;    //运行步数
// Create a new instance of the AccelStepper class:
//< Stepper Driver, 2 driver pins required
 AccelStepper stepper1 = AccelStepper(motorInterfaceType,xstepPin,xdirPin);//建立步进电机对象
 
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);
// const int led = 2;
bool running = false;

const String postForms = 
"<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"utf-8\">\
<title>Lynn's cockpit</title>\
<style>input[type=submit]{width: 100px;height: 50px;}\
form{margin:2px;display: inline}</style>\
</head>\
<body>\
<form action=\"/1/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"1\"></form>\   
<form action=\"/2/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"2\"></form>\
<form action=\"/3/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"3\"></form><br>\
<form action=\"/4/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"4\"></form>\   
<form action=\"/5/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"5\"></form>\
<form action=\"/6/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"6\"></form><br>\
<form action=\"/7/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"7\"></form>\   
<form action=\"/8/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"8\"></form>\
<form action=\"/9/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"9\"></form><br>\
<form action=\"/L/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"L\"></form>\
<form action=\"/R/\" method=\"post\" target=\"nm_iframe\"> <input type=\"submit\" value=\"R\"></form><br>\
<iframe id=\"id_iframe\" name=\"nm_iframe\" style=\"display:none;\"></iframe>\
</body>\
</html>";
 void handle5() {
   running = !running;
  digitalWrite(xdirPin, HIGH); //"Set run on clockwise
 }

void handle4() {
 running = !running;
 digitalWrite(xdirPin, LOW);//"Set run on anti-clockwise
// Serial.println(running);
 }

void handleRoot() {
  // digitalWrite(led, HIGH);
  server.send(200, "text/html", postForms);
  // digitalWrite(led, LOW);
}

// void handle1() {
//   for(int i=0; i < 1; i++){
//     digitalWrite(led, LOW);
//     delay(1000);
//     digitalWrite(led, HIGH);
//     delay(1000);
//   }
//   digitalWrite(led, LOW);
// }

// void handle2() {
// for(int i=0;i < 2; i++){
//     digitalWrite(led, LOW);
//     delay(1000);
//     digitalWrite(led, HIGH);
//     delay(1000);
//   }
//   digitalWrite(led, LOW);
// }

// void handle3() {
// for(int i=0;i < 3; i++){
//     digitalWrite(led, LOW);
//     delay(1000);
//     digitalWrite(led, HIGH);
//     delay(1000);
//   }
//   digitalWrite(led, LOW);
//  }


void setup(void) {
  // pinMode(led, OUTPUT);
  // digitalWrite(led, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // pinMode(stepPin, OUTPUT); //Step pin as output
  // pinMode(dirPin,  OUTPUT); //Direcction pin as output
  // digitalWrite(stepPin, LOW); // Set motor stop when initial power is on.
  // digitalWrite(dirPin, HIGH);  
  pinMode(xstepPin,OUTPUT);     // Arduino控制A4988步进引脚为输出模式
  pinMode(xdirPin,OUTPUT);      // Arduino控制A4988方向引脚为输出模式
  digitalWrite(xdirPin, HIGH);  
  pinMode(xenablePin,OUTPUT);   // Arduino控制A4988使能引脚为输出模式
  digitalWrite(xenablePin,LOW); // 将使能控制引脚设置为低电平从而让
                                // 电机驱动板进入工作状态
  stepper1.setMaxSpeed(1000.0);     // 设置电机最大速度300 
  stepper1.setAcceleration(100.0);  // 设置电机加速度20.0  
  // stepper1.setSpeed(700);

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

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }
  server.on("/", handleRoot);
  // server.on("/1/", handle1);
  // server.on("/2/", handle2);
  // server.on("/3/", handle3);
  server.on("/4/", handle4);//Stepper Motor control//"Set run on anti-clockwise
  server.on("/5/", handle5);//Stepper Motor control//"Set run on clockwise

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  if( running )
 {
  //  digitalWrite(stepPin,LOW);
   stepper1.stop();
  //  Serial.println("stepPin,LOW");
 }
  else
  {
  // digitalWrite(stepPin,HIGH);
  stepper1.enableOutputs();
  // Serial.println("stepPin,HIGH");
  stepper1.setCurrentPosition(0);

  // Run the motor forward at 200 steps/second until the motor reaches 400 steps (2 revolutions):
  // while(stepper.currentPosition() != 600)
  // {
    stepper1.setSpeed(-800);
    stepper1.runSpeed();
  // }
    // delay(2000); 
  }

  // // 控制步进电机往复运动
  // if ( stepper1.currentPosition() == 0 ){ 
  //   stepper1.moveTo(moveSteps);              
  // } else if ( stepper1.currentPosition() == moveSteps  ){
  //   stepper1.moveTo(0);            
  // }         
  
  // stepper1.run();   // 1号电机运行
}