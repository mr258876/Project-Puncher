#include <BasicStepperDriver.h>
#include <Wire.h>
#include "AS5600.h"
#include <TMCStepper.h>

#include "Project_HolePuncher_menu.h"
#include <PlatformDetermination.h>
#include <TaskManagerIO.h>

#include "PuncherScheduler.h"
#include "SerialCommand.h"

//----------------------------------------电机控制用常量
#define MOTOR_STEPS 200
#define MICROSTEPS 8

#define DRIVER_SERIAL Serial2 // TMC controller Serial Port
#define R_SENSE 0.11f         // Match to your driver <- ???

//----------------------------------------任务
void initTcMenu(void *pvParameters);
void initMotor(void *pvParameters);
void runTcMenu(void *pvParameters);
void serialCommand(void *pvParameters);
void punchSchedule(void *pvParameters);
void runStepper(void *pvParameters);
void runEncoder(void *pvParameters);

//----------------------------------------任务句柄
TaskHandle_t runTcMenu_Handler;
TaskHandle_t serialCommand_Handler;
TaskHandle_t runEncoder_Handler;

//----------------------------------------资源锁
SemaphoreHandle_t I2CMutex = xSemaphoreCreateMutex();       // I2C互斥量
SemaphoreHandle_t SPIMutex = xSemaphoreCreateMutex();       // SPI互斥量
SemaphoreHandle_t Serial0Mutex = xSemaphoreCreateMutex();   // 串口0互斥量
SemaphoreHandle_t Serial2Mutex = xSemaphoreCreateMutex();   // 串口2互斥量
SemaphoreHandle_t XaxisBinary = xSemaphoreCreateBinary();   // X轴信号量(同步用)
SemaphoreHandle_t YaxisBinary = xSemaphoreCreateBinary();   // Y轴信号量(同步用)
SemaphoreHandle_t ZaxisBinary = xSemaphoreCreateBinary();   // Z轴信号量(同步用)
SemaphoreHandle_t PuncherBinary = xSemaphoreCreateBinary(); // 打孔机信号量(同步用)

//----------------------------------------磁编码器
AS5600 encoderZ;
int lastAngle = -1;
long rotatedAngle = 0;
bool encoderDisabled = false;

//----------------------------------------电机控制变量
#define xenablePin 23 // x使能控制引脚
#define xdirPin 18    // x方向控制引脚
#define xstepPin 19   // x步进控制引脚

#define yenablePin 5 // y使能控制引脚
#define ydirPin 2    // y方向控制引脚
#define ystepPin 4   // y步进控制引脚

#define zenablePin 25 // z使能控制引脚
#define zdirPin 27    // z方向控制引脚
#define zstepPin 26   // z步进控制引脚

bool Xenabled = false;
bool Yenabled = false;
bool Zenabled = false;
bool Ydown = false;
bool isPunching = false;

unsigned stepper_to_wait;

int xRPM = 60; // 电机X速度
int yRPM = 30; // 电机Y速度
int zRPM = 45; // 电机Z速度

int xOptiCurr = 500; // 电机x工作电流
int xStbyCurr = 50;  // 电机x待机电流
int yOptiCurr = 500; // 电机y工作电流
int yStbyCurr = 50;  // 电机y待机电流
int zOptiCurr = 500; // 电机z工作电流
int zStbyCurr = 150; // 电机z待机电流

BasicStepperDriver stepperX(MOTOR_STEPS, xdirPin, xstepPin, xenablePin);
BasicStepperDriver stepperY(MOTOR_STEPS, ydirPin, ystepPin, yenablePin);
BasicStepperDriver stepperZ(MOTOR_STEPS, zdirPin, zstepPin, zenablePin);

TMC2209Stepper driverX(&DRIVER_SERIAL, R_SENSE, 0b00);
TMC2209Stepper driverY(&DRIVER_SERIAL, R_SENSE, 0b10);
TMC2209Stepper driverZ(&DRIVER_SERIAL, R_SENSE, 0b01);

long xPosition = 0;
long yPosition = 0;
long zLastMove = 0;

//-----------------------------------------tcMenu工具变量
char progressCA[9];

//-----------------------------------------
// 运行函数
// ----------------------------------------
void setup()
{
    Serial.begin(115200);
    Serial.println("Hole puncher booting...");

    // 为编码器启动I2C
    Wire.begin();
    Wire.setClock(400000);

    // 初始化tcMenu
    gfx.enableUTF8Print();    // 启用UTF-8以支持多语言
    gfx.setFontPosBaseline(); // 设置字体基线(虽然这里应该没啥用)
    setupMenu();
    installTheme();
    menuProgress.setTextValue("idle");
    menuMgr.navigateToMenu(menuInfoView.getChild());

    // 初始化电机
    pinMode(xstepPin, OUTPUT);
    pinMode(xdirPin, OUTPUT);
    pinMode(ystepPin, OUTPUT);
    pinMode(ydirPin, OUTPUT);
    pinMode(zstepPin, OUTPUT);
    pinMode(zdirPin, OUTPUT);

    pinMode(xenablePin, OUTPUT);
    pinMode(yenablePin, OUTPUT);
    pinMode(zenablePin, OUTPUT);

    digitalWrite(xenablePin, LOW); // 启用x方向电机
    digitalWrite(yenablePin, LOW); // 启用y方向电机
    digitalWrite(zenablePin, LOW); // 启用z方向电机

    stepperX.begin(xRPM, MICROSTEPS);
    stepperX.setEnableActiveState(LOW);
    stepperY.begin(yRPM, MICROSTEPS);
    stepperY.setEnableActiveState(LOW);
    stepperZ.begin(zRPM, MICROSTEPS);
    stepperZ.setEnableActiveState(LOW);
    stepperX.enable();
    stepperY.enable();
    stepperZ.enable();

    DRIVER_SERIAL.begin(115200);    // 启动串口
    driverX.begin();                // x方向驱动板开始通讯
    driverX.microsteps(MICROSTEPS); // x方向驱动板设置微步大小
//    driverX.rms_current(xStbyCurr); // x方向驱动板设置电流大小 (mA)
    driverY.begin();                // y方向驱动板开始通讯
    driverY.microsteps(MICROSTEPS); // y方向驱动板设置微步大小
//    driverY.rms_current(yStbyCurr); // y方向驱动板设置电流大小 (mA)
    driverZ.begin();                // z方向驱动板开始通讯
    driverZ.microsteps(MICROSTEPS); // z方向驱动板设置微步大小
//    driverZ.rms_current(zStbyCurr); // z方向驱动板设置电流大小 (mA)

    // 检测编码器状态
    Wire.beginTransmission(AS5600_I2C_ADDR);
    switch (Wire.endTransmission())
    {
    case 0:
        if (getMagnetStatus() == 1)
        {
            Serial.println("Endocer detected and enabled.");
            //------------------------------创建磁编码器任务
            xTaskCreate(runEncoder,           //任务函数
                        "runEncoder",         //任务名称
                        4096,                 //任务堆栈大小
                        NULL,                 //任务参数
                        1,                    //任务优先级
                        &runEncoder_Handler); //任务句柄
        }
        else
        {
            Serial.println("Magnet Error! Endocer Disabled!");
            Serial.print("Error code: ");
            Serial.println(getMagnetStatus());
            encoderDisabled = true;
        }
        break;
    default:
        Serial.println("Encoder Not Deceted and Disabled!");
        encoderDisabled = true;
        break;
    }

    // FreeRTOS创建任务
    //------------------------------创建tcMenu任务
    xTaskCreate(runTcMenu,           //任务函数
                "runTcMenu",         //任务名称
                8192,                //任务堆栈大小
                NULL,                //任务参数
                2,                   //任务优先级
                &runTcMenu_Handler); //任务句柄

    //------------------------------创建串口命令监听任务
    xTaskCreate(serialCommand,           //任务函数
                "serialCommand",         //任务名称
                8192,                    //任务堆栈大小
                NULL,                    //任务参数
                tskIDLE_PRIORITY,        //任务优先级
                &serialCommand_Handler); //任务句柄

    //------------------------------创建打孔调度任务
    // xTaskCreatePinnedToCore(punchSchedule,           //任务函数
    //                         "punchSchedule",         //任务名称
    //                         8192,                    //任务堆栈大小
    //                         NULL,                    //任务参数
    //                         1,                       //任务优先级
    //                         NULL, //任务句柄
    //                         0);                      //执行任务核心

    //------------------------------创建电机控制任务
    // xTaskCreatePinnedToCore(runStepper,       //任务函数
    //                         "runStepper",     //任务名称
    //                         8192,             //任务堆栈大小
    //                         NULL,             //任务参数
    //                         tskIDLE_PRIORITY, //任务优先级
    //                         NULL,             //任务句柄
    //                         1);               //执行任务核心

    //    vTaskStartScheduler(); //开启任务调度

    xSemaphoreGive(XaxisBinary);   //释放X轴同步信号量
    xSemaphoreGive(YaxisBinary);   //释放Y轴同步信号量
    xSemaphoreGive(ZaxisBinary);   //释放Z轴同步信号量
    xSemaphoreGive(PuncherBinary); //释放打孔机同步信号量

    Serial.println("Puncher booted.");
}

// 电机运行及调度现由loop函数处理 DEV.20220528
void loop()
{
    //    Serial.println(encoderZ.getAngle());
    if (!isPunching && puncherStatus == 0x11 && holeList.size() > 0) // 当状态为正在打孔且打孔队列有孔时才进行调度
    {
        isPunching = true;
        Hole h = holeList.shift(); // 获取一个孔
        moveXpos(h.x);             // 移动X轴
        moveZ(h.z);                // 移动Z轴

        (String(holeFinished) + "/" + String(holeCount)).toCharArray(progressCA, 9);
        menuProgress.setTextValue(progressCA);
        menuETA.setTime(secToTime(calcETA(xRPM, yRPM, zRPM)));
        menuETA.setChanged(true);
    }

    if (Xenabled)
    {
        stepper_to_wait = stepperX.nextAction();
        if (stepper_to_wait <= 0)
        {
            Xenabled = false;
            startYaxis();
        }
    }

    if (Zenabled)
    {
        stepper_to_wait = stepperZ.nextAction();
        if (stepper_to_wait <= 0)
        {
            Zenabled = false;
            startYaxis();
        }
    }

    if (Yenabled)
    {
        stepper_to_wait = stepperY.nextAction();
        if (stepper_to_wait <= 0)
        {
            if (Ydown)
            {
                moveYto(0); // 若Y轴已移动至下方则回到初始位置
                Ydown = false;
            }
            else
            {
                Yenabled = false;
                isPunching = false;

                holeFinished += 1;
            }
        }
    }
}

// TcMenu 主题设置
void installTheme()
{
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(false);
    menuProgress.setTextValue("");
    menuETA.setTime(secToTime(0));
    menuDuration.setTime(secToTime(0));
}

//------------------------------TcMenu 主进程循环函数
void runTcMenu(void *pvParameters)
{
    Serial.println();
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        if (xSemaphoreTake(I2CMutex, portMAX_DELAY) == pdTRUE)
        {
            taskManager.runLoop();
            xSemaphoreGive(I2CMutex);
        }

        if (punchStartTime > 0)
        {
            menuDuration.setTime(secToTime((int)(millis() - punchStartTime) / 1000));
            menuDuration.setChanged(true);
        }
    }
}

//------------------------------串口监听函数
void serialCommand(void *pvParameters)
{
    byte serBuf[8] = {};
    while (1)
    {
        if (xSemaphoreTake(Serial0Mutex, portMAX_DELAY) == pdTRUE)
        {
            while (Serial.available() > 7)
            {
                Serial.readBytes(serBuf, 8);
                handleSerialCommand(serBuf, 8);
            }
            xSemaphoreGive(Serial0Mutex);
        }
    }
}

//-----------------------------------------
// 电机控制函数
// ----------------------------------------
void startYaxis() // X,Z轴移动完成后控制Y轴向下移动打孔
{
    if (!Xenabled && !Zenabled)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        if (abs(rotatedAngle / 4096.0 * MOTOR_STEPS * MICROSTEPS + zLastMove) < 1 || encoderDisabled)
        {
            moveYto(4);
            Ydown = true;
        }
        else
        {
            Zenabled = true;
            stepperZ.startMove((long)(zLastMove + rotatedAngle / 4096.0 * MOTOR_STEPS * MICROSTEPS));
        }
    }
}

void moveZ(float mm)
{
    Zenabled = true;
    //     driverZ.rms_current(zOptiCurr);
    long toMove = (long)(-mm * 25.852579588531222053828834659495);
    zLastMove = toMove;
    stepperZ.startMove(toMove);
    //     driverZ.rms_current(zStbyCurr);
}

void moveXpos(int pos)
{
    if (pos == -1)
    {
        moveXto(0);
    }
    else
    {
        moveXto(2 * (pos - 29) - 3.55 - 0.3);
    }
}

void moveXto(float mm)
{
    Xenabled = true;
    //    driverX.rms_current(xOptiCurr);
    long steps = (long)(mm / 8.0 * (200 * 8) + 0.5);
    long toMove = xPosition - steps;
    stepperX.startMove(toMove);
    //    driverX.rms_current(xStbyCurr);
    xPosition = steps;
}

void moveYto(float mm)
{
    Yenabled = true;
    //    driverY.rms_current(yOptiCurr);
    long steps = (long)(mm / 8.0 * (200 * 8) + 0.5);
    long toMove = steps - yPosition;
    stepperY.startMove(toMove);
    //    driverY.rms_current(yStbyCurr);
    yPosition = steps;
}

//----------------------------------------
// 磁编码器
//----------------------------------------
// 获取磁铁状态
// 返回值: -1  未检测到磁铁
// 返回值: 0   磁铁磁力过弱（距离过远）
// 返回值: 1   状态正常
// 返回值: 2   磁铁磁力过强（距离过近）
uint8_t getMagnetStatus()
{
    if (encoderZ.getMagnitude() < 750)
    {
        return 255;
    }

    AS5600_Status s = encoderZ.getStatus();
    if (s.magnetHigh)
    {
        return 2;
    }
    else if (s.magnetLow)
    {
        return 0;
    }
    return 1;
}

// 编码器运行函数
void runEncoder(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(I2CMutex, portMAX_DELAY) == pdTRUE)
        {
            int v = encoderZ.getAngle();
            if (lastAngle != -1)
            {
                if (abs(v - lastAngle) > 2047)
                {
                    rotatedAngle += v > 2048 ? v - lastAngle - 4096 : v - lastAngle + 4096;
                }
                else
                {
                    rotatedAngle += v - lastAngle;
                }
            }
            lastAngle = v;
            xSemaphoreGive(I2CMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//----------------------------------------
// 工具函数
//----------------------------------------
// 秒数转时间
TimeStorage secToTime(int sec)
{
    return (TimeStorage{sec / 3600, sec % 3600 / 60, sec % 60, 0});
}
