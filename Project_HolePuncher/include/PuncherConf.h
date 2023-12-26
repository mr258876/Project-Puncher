#ifndef _PUNCHERCONF_H_
#define _PUNCHERCONF_H_

//---------------- Motor Controls ----------------
#define MOTOR_STEPS     200
#define MICROSTEPS_X    64
#define MICROSTEPS_Y    64
#define MICROSTEPS_Z    64

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

#define X_ZEROING_POSITION      -5.55
#define Y_ZEROING_POSITION      -7.00
#define Y_PUNCH_MOVEMENT_LENGTH 3.00

#define LCD_LEDC_CHANNEL 7

#endif
