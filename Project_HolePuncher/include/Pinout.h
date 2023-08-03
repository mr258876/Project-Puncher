#ifndef _PINOUT_H_
#define _PINOUT_H_

#define xenablePin 5 // x使能控制引脚
#define xdirPin 2    // x方向控制引脚
#define xstepPin 4   // x步进控制引脚
#define xdiagPin 33  // x方向过载引脚

#define yenablePin 23 // y使能控制引脚
#define ydirPin 18    // y方向控制引脚
#define ystepPin 19   // y步进控制引脚
#define ydiagPin 39   // y方向过载引脚

#define zenablePin 25 // z使能控制引脚
#define zdirPin 27    // z方向控制引脚
#define zstepPin 26   // z步进控制引脚
#define zdiagPin 36   // z方向过载引脚

//----------------------------------------电机控制用常量
#define MOTOR_STEPS 200
#define MICROSTEPS_X 16
#define MICROSTEPS_Y 8
#define MICROSTEPS_Z 16

#define DRIVER_SERIAL Serial2 // TMC controller Serial Port
#define R_SENSE 0.11f         // Match to your driver <- ???

#endif  //_PINOUT_H_