#include <AccelStepper.h>  //本示例程序使用AccelStepper库
 
// 定义电机控制用常量
const int enablePin = 8;  // 使能控制引脚
 
const int xdirPin = 5;     // x方向控制引脚
const int xstepPin = 2;    // x步进控制引脚
const int ydirPin = 6;     // y方向控制引脚
const int ystepPin = 3;    // y步进控制引脚
const int zdirPin = 7;     // z方向控制引脚
const int zstepPin = 4;    // z步进控制引脚
const int adirPin = 13;     // a方向控制引脚
const int astepPin = 12;    // a步进控制引脚
 
const int moveSteps = 200;    //测试电机运行使用的运行步数
 
AccelStepper stepper1(1,xstepPin,xdirPin);//建立步进电机对象1
AccelStepper stepper2(1,ystepPin,ydirPin);//建立步进电机对象2
AccelStepper stepper3(1,zstepPin,zdirPin);//建立步进电机对象3
AccelStepper stepper4(1,astepPin,adirPin);//建立步进电机对象4
 
void setup() {
  
  pinMode(xstepPin,OUTPUT);     // Arduino控制A4988x步进引脚为输出模式
  pinMode(xdirPin,OUTPUT);      // Arduino控制A4988x方向引脚为输出模式
  pinMode(ystepPin,OUTPUT);     // Arduino控制A4988y步进引脚为输出模式
  pinMode(ydirPin,OUTPUT);      // Arduino控制A4988y方向引脚为输出模式
  pinMode(zstepPin,OUTPUT);     // Arduino控制A4988z步进引脚为输出模式
  pinMode(zdirPin,OUTPUT);      // Arduino控制A4988z方向引脚为输出模式
  pinMode(astepPin,OUTPUT);     // Arduino控制A4988a步进引脚为输出模式
  pinMode(adirPin,OUTPUT);      // Arduino控制A4988a方向引脚为输出模式  
  
  pinMode(enablePin,OUTPUT);   // Arduino控制A4988使能引脚为输出模式
  digitalWrite(enablePin,LOW); // 将使能控制引脚设置为低电平从而让
                               // 电机驱动板进入工作状态
                                
  stepper1.setMaxSpeed(300.0);     // 设置电机最大速度300 
  stepper1.setAcceleration(20.0);  // 设置电机加速度20.0  
  stepper2.setMaxSpeed(300.0);     // 设置电机最大速度300 
  stepper2.setAcceleration(20.0);  // 设置电机加速度20.0 
  stepper3.setMaxSpeed(300.0);     // 设置电机最大速度300 
  stepper3.setAcceleration(20.0);  // 设置电机加速度20.0 
  stepper4.setMaxSpeed(300.0);     // 设置电机最大速度300 
  stepper4.setAcceleration(20.0);  // 设置电机加速度20.0   
}
 
void loop() {
  // 控制步进电机1往复运动
  if ( stepper1.currentPosition() == 0 ){ 
    stepper1.moveTo(moveSteps);              
  } else if ( stepper1.currentPosition() == moveSteps  ){
    stepper1.moveTo(0);            
  }  
  
  // 控制步进电机2往复运动
  if ( stepper2.currentPosition() == 0 ){ 
    stepper2.moveTo(moveSteps/2);              
  } else if ( stepper2.currentPosition() == moveSteps/2  ){
    stepper2.moveTo(0);            
  }    
          
  // 控制步进电机3往复运动
  if ( stepper3.currentPosition() == 0 ){ 
    stepper3.moveTo(moveSteps/4);              
  } else if ( stepper3.currentPosition() == moveSteps/4  ){
    stepper3.moveTo(0);            
  }     
 
  // 控制步进电机4往复运动
  if ( stepper4.currentPosition() == 0 ){ 
    stepper4.moveTo(moveSteps/8);              
  } else if ( stepper4.currentPosition() == moveSteps/8  ){
    stepper4.moveTo(0);            
  }
    
  stepper1.run();   // 1号电机运行
  stepper2.run();   // 2号电机运行
  stepper3.run();   // 3号电机运行
  stepper4.run();   // 3号电机运行
}