#ifndef _PUNCHERCONF_H_
#define _PUNCHERCONF_H_

//---------------- Motor Controls ----------------
#define MOTOR_STEPS     200
#define MICROSTEPS_X    16
#define MICROSTEPS_Y    8
#define MICROSTEPS_Z    16

#define DRIVER_SERIAL Serial // TMC controller Serial Port
#define R_SENSE 0.11f         // Match to your driver <- ???
#define DRIVER_UART_BANDRATE    115200

#define xenablePin  46  // x使能控制引脚
#define xdirPin     3   // x方向控制引脚
#define xstepPin    9   // x步进控制引脚
#define xdiagPin    13  // x方向过载引脚

#define yenablePin  0   // y使能控制引脚
#define ydirPin     45  // y方向控制引脚
#define ystepPin    40  // y步进控制引脚
#define ydiagPin    39  // y方向过载引脚

#define zenablePin  42  // z使能控制引脚
#define zdirPin     1   // z方向控制引脚
#define zstepPin    2   // z步进控制引脚
#define zdiagPin    41  // z方向过载引脚

#define X_CONTROL_CHANNEL 0
#define Y_CONTROL_CHANNEL 1
#define Z_CONTROL_CHANNEL 2

#define LCD_LEDC_CHANNEL 7

#endif
