#include <BasicStepperDriver.h>
#include <Wire.h>
#include "AS5600.h"
#include <TMCStepper.h>

#include <WiFi.h>
#include <EEPROM.h>

#include "Project_HolePuncher_menu.h"
#include <PlatformDetermination.h>
#include <TaskManagerIO.h>

#include "PuncherScheduler.h"
#include "SerialCommand.h"

#include "lang_def.h"

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
TaskHandle_t runTcMenu_Handle;
TaskHandle_t serialCommand_Handle;
TaskHandle_t runEncoder_Handle;
TaskHandle_t wifiConnect_Handle;
TaskHandle_t wifiCommand_Handle;
TaskHandle_t punchSchedule_Handle;

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
bool encoderDisabled = true;

//----------------------------------------无线网络
#define WIFI_CONTROL_PORT 22576

int SSIDCount = 0;
WiFiServer server(WIFI_CONTROL_PORT);

//----------------------------------------电机控制变量
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

bool Xenabled = false;
bool Yenabled = false;
bool Zenabled = false;
bool Ydown = false;
bool isPunching = false;

unsigned stepperX_to_wait;
unsigned stepperY_to_wait;
unsigned stepperZ_to_wait;

int xOptiCurr = 500; // 电机x工作电流
int xStbyCurr = 50;  // 电机x待机电流
int yOptiCurr = 500; // 电机y工作电流
int yStbyCurr = 50;  // 电机y待机电流
int zOptiCurr = 500; // 电机z工作电流
int zStbyCurr = 150; // 电机z待机电流

double xPerimeter = 0; // x轴转动周长
double yPerimeter = 0; // y轴转动周长
double zPerimeter = 0; // z轴转动周长

BasicStepperDriver stepperX(MOTOR_STEPS, xdirPin, xstepPin, xenablePin);
BasicStepperDriver stepperY(MOTOR_STEPS, ydirPin, ystepPin, yenablePin);
BasicStepperDriver stepperZ(MOTOR_STEPS, zdirPin, zstepPin, zenablePin);

TMC2209Stepper driverX(&DRIVER_SERIAL, R_SENSE, 0b00);
TMC2209Stepper driverY(&DRIVER_SERIAL, R_SENSE, 0b01);
TMC2209Stepper driverZ(&DRIVER_SERIAL, R_SENSE, 0b10);

long xPosition = 0;
long yPosition = 0;
long zLastMove = 0;

double lengthZ = 0; // z轴位置

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

    checkReset(); // 检查是否需要设置初始值

    menuProgress.setTextValue("idle"); // 将进度文字设为idle
    renderer.turnOffResetLogic();      // 关闭闲置回调函数
    // menuMgr.navigateToMenu(menuInfoView.getChild());

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

    pinMode(xdiagPin, INPUT);
    pinMode(ydiagPin, INPUT);
    pinMode(zdiagPin, INPUT);

    digitalWrite(xenablePin, LOW); // 启用x方向电机
    digitalWrite(yenablePin, LOW); // 启用y方向电机
    digitalWrite(zenablePin, LOW); // 启用z方向电机

    getStepperPerimeters();

    stepperX.begin(menuRunningSpeedX.getAsFloatingPointValue() / xPerimeter * 60, MICROSTEPS);
    stepperX.setEnableActiveState(LOW);
    stepperY.begin(menuRunningSpeedY.getAsFloatingPointValue() / yPerimeter * 60, MICROSTEPS);
    stepperY.setEnableActiveState(LOW);
    stepperZ.begin(menuRunningSpeedZ.getAsFloatingPointValue() / zPerimeter * 60, MICROSTEPS);
    stepperZ.setEnableActiveState(LOW);
    stepperX.enable();
    stepperY.enable();
    stepperZ.enable();

    DRIVER_SERIAL.begin(115200);                                // 启动串口
    driverX.begin();                                            // x方向驱动板开始通讯
    driverX.microsteps(MICROSTEPS);                             // x方向驱动板设置微步大小
    driverX.rms_current(menuRunningCurrentX.getCurrentValue()); // x方向驱动板设置电流大小 (mA)
    driverY.begin();                                            // y方向驱动板开始通讯
    driverY.microsteps(MICROSTEPS);                             // y方向驱动板设置微步大小
    driverY.rms_current(menuRunningCurrentY.getCurrentValue()); // y方向驱动板设置电流大小 (mA)
    driverZ.begin();                                            // z方向驱动板开始通讯
    driverZ.microsteps(MICROSTEPS);                             // z方向驱动板设置微步大小
    driverZ.rms_current(menuRunningCurrentZ.getCurrentValue()); // z方向驱动板设置电流大小 (mA)

    // FreeRTOS创建任务
    //------------------------------创建tcMenu任务
    xTaskCreate(runTcMenu,          //任务函数
                "runTcMenu",        //任务名称
                8192,               //任务堆栈大小
                NULL,               //任务参数
                2,                  //任务优先级
                &runTcMenu_Handle); //任务句柄

    //------------------------------创建串口命令监听任务
    xTaskCreate(serialCommand,          //任务函数
                "serialCommand",        //任务名称
                8192,                   //任务堆栈大小
                NULL,                   //任务参数
                tskIDLE_PRIORITY,       //任务优先级
                &serialCommand_Handle); //任务句柄

    //------------------------------创建调度任务
    xTaskCreate(punchSchedule,          //任务函数
                "punchSchedule",        //任务名称
                8192,                   //任务堆栈大小
                NULL,                   //任务参数
                tskIDLE_PRIORITY,       //任务优先级
                &punchSchedule_Handle); //任务句柄

    xSemaphoreGive(XaxisBinary);   //释放X轴同步信号量
    xSemaphoreGive(YaxisBinary);   //释放Y轴同步信号量
    xSemaphoreGive(ZaxisBinary);   //释放Z轴同步信号量
    xSemaphoreGive(PuncherBinary); //释放打孔机同步信号量

    //--------------------连接Wifi--------------------
    WiFi.mode(WIFI_STA);
    SSIDCount = WiFi.scanNetworks();
    menuAllNetworks.setNumberOfRows(SSIDCount);
    menuSSID.setTextValue("N/A");
    menuIP.setIpAddress("127.0.0.1");
    if (menuWifi.getCurrentValue() == true)
    {
        //------------------------------创建连接Wifi任务
        xTaskCreate(wifiConnect,          //任务函数
                    "wifiConnect",        //任务名称
                    8192,                 //任务堆栈大小
                    NULL,                 //任务参数
                    tskIDLE_PRIORITY,     //任务优先级
                    &wifiConnect_Handle); //任务句柄
    }

    //--------------------检测编码器状态并启用编码器--------------------
    if (menuUseEncoderZ.getCurrentValue())
    {
        // openDialogNoButton(TEXT_ATTENTION, TEXT_CONNECTING_ENCODER, TEXT_EMPTY);
        Wire.beginTransmission(AS5600_I2C_ADDR);
        switch (Wire.endTransmission())
        {
        case 0:
            if (getMagnetStatus() == 1)
            {
                encoderDisabled = false;
                // Serial.println("Endocer detected and enabled.");
                //------------------------------创建磁编码器任务
                xTaskCreate(runEncoder,          //任务函数
                            "runEncoder",        //任务名称
                            4096,                //任务堆栈大小
                            NULL,                //任务参数
                            3,                   //任务优先级
                            &runEncoder_Handle); //任务句柄
            }
            else
            {
                // Serial.println("Magnet Error! Endocer Disabled!");
                // Serial.print("Error code: ");
                // Serial.println(getMagnetStatus());
                openDialog(TEXT_ERROR, TEXT_MAGNET_NOT_DETECTED, TEXT_ENCODER_NOW_DISABLED);
            }
            break;
        default:
            // Serial.println("Encoder Not Deceted and Disabled!");
            openDialog(TEXT_ERROR, TEXT_ENCODER_NOT_DECETED, TEXT_ENCODER_NOW_DISABLED);
            break;
        }
    }

    //--------------------X/Y轴归零--------------------
    if (menuVirtualEndstopX.getBoolean())
    {
        // 中断
        // attachInterrupt(digitalPinToInterrupt(xdiagPin), onInterruptX, RISING);

        vTaskSuspend(punchSchedule_Handle); // 暂停调度任务

        uint8_t sgThrs = menuEndstopThresholdX.getCurrentValue();
        int sgReadings[] = {509, 509, 509, 509, 509, 509, 509, 509, 509, 509};
        int readingIndex = 0;
        int readingSum = 0;

        openDialogNoButton(TEXT_ATTENTION, TEXT_X_AXIS, TEXT_ZEROING);

        driverX.rms_current(250);
        // driverX.SGTHRS(menuEndstopThresholdX.getCurrentValue());
        stepperX.startMove(-99999);
        stepperX.setRPM(20);
        while (stepperX.nextAction() > 0)
        {   
            // Serial.println(driverX.SG_RESULT());
            // 10个读数取平均
            if (driverX.SG_RESULT() > 0)
            {
                sgReadings[readingIndex] = driverX.SG_RESULT();
                readingIndex++;
            }
            if (readingIndex > 9)
            {
                readingIndex = 0;
            }
            readingSum = 0;
            for (int i = 0; i < 10; i++)
            {
                readingSum += sgReadings[i];
            }

            if (readingSum / 10 > 0 && readingSum / 10 < sgThrs * 2) // SG_RESULT小于阈值两倍时代表电机过载，详见tmc209手册
            {
                stepperX.startMove(0);
                stepperX.setRPM(menuRunningSpeedX.getAsFloatingPointValue() / xPerimeter * 60);
                driverX.rms_current(menuRunningCurrentX.getCurrentValue());
                break;
            }
        }

        vTaskResume(punchSchedule_Handle);  // 重启调度任务
        closeDialog();
    }

    if (menuVirtualEndstopY.getBoolean())
    {
        // 中断
        // attachInterrupt(digitalPinToInterrupt(xdiagPin), onInterruptX, RISING);
        
        vTaskSuspend(punchSchedule_Handle); // 暂停调度任务
        
        uint8_t sgThrs = menuEndstopThresholdY.getCurrentValue();
        int sgReadings[] = {509, 509, 509, 509, 509, 509, 509, 509, 509, 509};
        int readingIndex = 0;
        int readingSum = 0;

        openDialogNoButton(TEXT_ATTENTION, TEXT_Y_AXIS, TEXT_ZEROING);

        driverY.rms_current(250);
        // driverY.SGTHRS(menuEndstopThresholdY.getCurrentValue());
        stepperY.startMove(-99999);
        stepperY.setRPM(20);
        while (stepperY.nextAction() > 0)
        {   
            // Serial.println(driverY.SG_RESULT());
            // 10个读数取平均
            if (driverY.SG_RESULT() > 0)
            {
                sgReadings[readingIndex] = driverY.SG_RESULT();
                readingIndex++;
            }
            if (readingIndex > 9)
            {
                readingIndex = 0;
            }
            readingSum = 0;
            for (int i = 0; i < 10; i++)
            {
                readingSum += sgReadings[i];
            }

            if (readingSum / 10 > 0 && readingSum / 10 < sgThrs * 2) // SG_RESULT小于阈值两倍时代表电机过载，详见tmc209手册
            {
                stepperY.startMove(0);
                stepperY.setRPM(menuRunningSpeedY.getAsFloatingPointValue() / yPerimeter * 60);
                driverY.rms_current(menuRunningCurrentY.getCurrentValue());
                moveYto(6);
                yPosition = 0;
                break;
            }
        }

        vTaskResume(punchSchedule_Handle);  // 重启调度任务
        closeDialog();
    }

    // Serial.println("Puncher booted.");

    // 启动完成后切换至主菜单
    // menuMgr.navigateToMenu(&menuProgress);
}

// 电机运行现由loop函数处理 DEV.20220718
void loop()
{
    stepperX_to_wait = stepperX.nextAction();
    stepperY_to_wait = stepperY.nextAction();
    stepperZ_to_wait = stepperZ.nextAction();
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

// 默认值设置
void checkReset()
{
    if (!menuResetStatus.getCurrentValue())
    {
        menuLengthX.setCurrentValue(800);
        menuLengthY.setCurrentValue(800);
        menuLengthZ.getLargeNumber()->setFromFloat(20.0);

        menuLengthTypeX.setCurrentValue(0);
        menuLengthTypeY.setCurrentValue(0);
        menuLengthTypeZ.setCurrentValue(0);

        menuDiamRatio.getLargeNumber()->setFromFloat(0);

        menuRunningSpeedX.setCurrentValue(120);
        menuRunningSpeedY.setCurrentValue(80);
        menuRunningSpeedZ.setCurrentValue(400);

        menuRunningCurrentX.setCurrentValue(500);
        menuRunningCurrentY.setCurrentValue(800);
        menuRunningCurrentZ.setCurrentValue(500);

        menuVirtualEndstopX.setBoolean(false);
        menuVirtualEndstopY.setBoolean(false);

        menuEndstopThresholdX.setCurrentValue(15);
        menuEndstopThresholdY.setCurrentValue(5);

        menuUseEncoderZ.setBoolean(false);

        menuWifi.setBoolean(false);

        menuResetStatus.setBoolean(true);
        saveValues(0);
    }
}

// 计算周长
void getStepperPerimeters()
{
    xPerimeter = menuLengthX.getAsFloatingPointValue() * (menuLengthTypeX.getCurrentValue() == 0 ? 1 : 3.14159265358979);
    yPerimeter = menuLengthY.getAsFloatingPointValue() * (menuLengthTypeY.getCurrentValue() == 0 ? 1 : 3.14159265358979);
    zPerimeter = menuLengthZ.getLargeNumber()->getAsFloat() * (menuLengthTypeZ.getCurrentValue() == 0 ? 1 : 3.14159265358979);
}

//------------------------------TcMenu 主进程循环函数
void runTcMenu(void *pvParameters)
{
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
    byte serBuf[16] = {};
    unsigned long lastConn = millis();
    while (1)
    {
        if (xSemaphoreTake(Serial0Mutex, portMAX_DELAY) == pdTRUE)
        {
            while (Serial.available() > 15)
            {
                Serial.readBytes(serBuf, 16);
                Serial.write(handleSerialCommand(serBuf, 16), 16);
                lastConn = millis();
            }

            if (isSerialCommandEnabled())
            {
                // 30秒发一个心跳包
                if (millis() - lastConn > 30000)
                {
                    Serial.write(HeartBeatPackage, 16);
                }
                // 5秒内无回应关闭通讯
                if (millis() - lastConn > 35000)
                {
                    disableSerialCommand();
                }
            }

            xSemaphoreGive(Serial0Mutex);
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

//-----------------------------------------
// 打孔调度函数
// ----------------------------------------
void punchSchedule(void *pvParameters)
{
    while (1)
    {
        //    Serial.println(encoderZ.getAngle());
        if (!isPunching && puncherStatus == 0x11 && holeList.size() > 0) // 当状态为正在打孔且打孔队列有孔时才进行调度
        {
            // 计算剩余时间
            // 注意：calcETA需要访问holeList, 因此必须放在holeList.shift()前，否则在list size == 1时将会产生null pointer exception
            (String(holeFinished + 1) + "/" + String(holeCount)).toCharArray(progressCA, 9);
            menuProgress.setTextValue(progressCA);
            menuETA.setTime(secToTime(calcETA(menuRunningSpeedX.getAsFloatingPointValue(), menuRunningSpeedY.getAsFloatingPointValue(), menuRunningSpeedZ.getAsFloatingPointValue())));
            menuETA.setChanged(true);

            isPunching = true;
            Hole h = holeList.shift(); // 获取一个孔
            moveXpos(h.x);             // 移动X轴
            moveZ(h.z - lengthZ);      // 移动Z轴

            lengthZ = h.z;
        }
        else if (!isPunching && puncherStatus == 0x11 && holeList.size() == 0)
        {
            lengthZ = 0;
            rotatedAngle = 0;
            puncherStatus = 0x10;

            menuProgress.setTextValue("idle");
            menuETA.setTime(secToTime(0));
            menuETA.setChanged(true);
            menuDuration.setTime(secToTime(0));
            punchStartTime = 0;
        }

        if (Xenabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            if (stepperX_to_wait <= 0)
            {
                Xenabled = false;
                startYaxis();
            }
        }

        if (Zenabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            if (stepperZ_to_wait <= 0)
            {
                Zenabled = false;
                startYaxis();
            }
        }

        if (Yenabled)
        {
            vTaskDelay(pdMS_TO_TICKS(100));
            if (stepperY_to_wait <= 0)
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
}

//-----------------------------------------
// 电机控制函数
// ----------------------------------------
void startYaxis() // X,Z轴移动完成后控制Y轴向下移动打孔
{
    if (!Xenabled && !Zenabled)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        if (encoderDisabled || abs(rotatedAngle / (4096 * menuDiamRatio.getLargeNumber()->getAsFloat()) * MOTOR_STEPS * MICROSTEPS + zLastMove) < 1)
        {
            moveYto(5.5);
            Ydown = true;
            rotatedAngle = 0;
        }
        else
        {
            Zenabled = true;
            stepperZ.startMove((long)(zLastMove + rotatedAngle / (4096.0 * menuDiamRatio.getLargeNumber()->getAsFloat()) * MOTOR_STEPS * MICROSTEPS));
            // Serial.println(rotatedAngle / (4096 * menuDiamRatio.getLargeNumber()->getAsFloat()) * MOTOR_STEPS * MICROSTEPS + zLastMove);
        }
    }
}

void moveZ(double mm)
{
    Zenabled = true;
    //     driverZ.rms_current(zOptiCurr);
    long toMove = (long)(-mm / zPerimeter * MOTOR_STEPS * MICROSTEPS);
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
        moveXto(2 * (pos - 29) - 3.9);
    }
}

void moveXto(double mm)
{
    Xenabled = true;
    //    driverX.rms_current(xOptiCurr);
    long steps = (long)(mm / xPerimeter * (MOTOR_STEPS * MICROSTEPS) + 0.5);
    long toMove = xPosition - steps;
    stepperX.startMove(toMove);
    //    driverX.rms_current(xStbyCurr);
    xPosition = steps;
}

void moveYto(double mm)
{
    Yenabled = true;
    //    driverY.rms_current(yOptiCurr);
    long steps = (long)(mm / yPerimeter * (MOTOR_STEPS * MICROSTEPS) + 0.5);
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
        while (xSemaphoreTake(I2CMutex, portMAX_DELAY) != pdTRUE)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        int v = encoderZ.getAngle();
        // v += encoderZ.getAngle();
        // v += encoderZ.getAngle();
        // v = v / 3;
        if (lastAngle != -1)
        {
            if (abs(v - lastAngle) > 2048)
            {
                rotatedAngle += v > 2048 ? v + lastAngle - 4096 : v - lastAngle + 4096;
            }
            else
            {
                rotatedAngle += v - lastAngle;
            }
        }
        else
        {
            rotatedAngle = 0;
        }

        lastAngle = v;
        xSemaphoreGive(I2CMutex);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// 编码器校准
void calibrateEncoder(void *pvParameters)
{
    // 若编码器不空闲则报错
    if (puncherStatus != 0x10)
    {
        openDialog(TEXT_ERROR, TEXT_PUNCHER_BUSY, TEXT_EMPTY);
        vTaskDelete(NULL);
    }

    openDialogNoButton(TEXT_ATTENTION, TEXT_CALIBRATING_ENCODER, TEXT_EMPTY);

    vTaskSuspend(runEncoder_Handle);    // 暂停编码器任务
    vTaskSuspend(punchSchedule_Handle); // 暂停调度任务
    puncherStatus = 0x11;

    uint16_t angleReading[MOTOR_STEPS * 2];
    uint16_t lastAngle = encoderZ.getAngle();
    uint16_t readingA = 0;
    uint16_t readingB = 0;
    uint16_t readingTemp = 0;
    // 正反转各一圈获取读数
    // 正转
    for (int i = 0; i < MOTOR_STEPS; i++)
    {   
        readingTemp = 0;
        stepperZ.startMove(MICROSTEPS);
        while (stepperZ_to_wait)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        while (xSemaphoreTake(I2CMutex, portMAX_DELAY) != pdTRUE)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // readingTemp += encoderZ.getAngle();
        // readingTemp += encoderZ.getAngle();
        // readingTemp += encoderZ.getAngle();
        // angleReading[i] = readingTemp / 2;
        angleReading[i] = encoderZ.getAngle();

        xSemaphoreGive(I2CMutex);

        vTaskDelay(pdMS_TO_TICKS(100));

        Serial.println(angleReading[i] - lastAngle);
        if (abs(angleReading[i] - lastAngle) > 2048)
        {
            readingA += angleReading[i] > 2048 ? abs(angleReading[i] + lastAngle - 4096) : abs(angleReading[i] - lastAngle + 4096);
        }
        else
        {
            readingA += abs(angleReading[i] - lastAngle);
        }
        lastAngle = angleReading[i];
    }
    // 反转
    for (int i = 0; i < MOTOR_STEPS; i++)
    {   
        readingTemp = 0;
        stepperZ.startMove(-MICROSTEPS);
        while (stepperZ_to_wait)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        while (xSemaphoreTake(I2CMutex, portMAX_DELAY) != pdTRUE)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        
        // readingTemp += encoderZ.getAngle();
        // readingTemp += encoderZ.getAngle();
        // readingTemp += encoderZ.getAngle();
        // angleReading[MOTOR_STEPS + i] = readingTemp / 2;
        angleReading[MOTOR_STEPS + i] = encoderZ.getAngle();

        xSemaphoreGive(I2CMutex);

        vTaskDelay(pdMS_TO_TICKS(100));

        Serial.println(angleReading[MOTOR_STEPS + i] - lastAngle);
        if (abs(angleReading[MOTOR_STEPS + i] - lastAngle) > 2048)
        {
            readingA += angleReading[MOTOR_STEPS + i] > 2048 ? abs(angleReading[MOTOR_STEPS + i] + lastAngle - 4096) : abs(angleReading[MOTOR_STEPS + i] - lastAngle + 4096);
        }
        else
        {
            readingB += abs(angleReading[MOTOR_STEPS + i] - lastAngle);
        }
        lastAngle = angleReading[MOTOR_STEPS + i];
    }
    rotatedAngle = 0;

    // 获取主动轮/被动轮间周长比值
    menuDiamRatio.getLargeNumber()->setFromFloat((readingA + readingB) / 2.0 / 4096);
    // Serial.println((readingA + readingB) / 2.0 / 4096);

    puncherStatus = 0x10;
    // 重启编码器任务
    vTaskResume(runEncoder_Handle);
    vTaskResume(punchSchedule_Handle);

    closeDialog();

    saveValues(0);

    vTaskDelete(NULL);
}

//----------------------------------------
// Wifi 相关
//----------------------------------------
// 自动连接至网络
void wifiConnect(void *pvParameters)
{
    menuWifiStatus.setTextValue("Connecting");
    menuSSID.setTextValue("N/A");
    menuIP.setIpAddress("127.0.0.1");

    const char *wifiSSID = getStringEEPROM(301);
    const char *wifiPswd = getStringEEPROM(331);
    switch (EEPROM.read(300))
    {
    case 0x01:
        // Serial.println("Open wifi");
        WiFi.begin(wifiSSID);
        break;
    case 0x02:
        // Serial.println("WPA wifi");
        // Serial.println(wifiSSID);
        // Serial.println(wifiPswd);
        WiFi.begin(wifiSSID, wifiPswd);
        break;
    default:
        return;
    }

    // Serial.print("try to connect");
    unsigned long t = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        // Serial.print(".");
        vTaskDelay(pdMS_TO_TICKS(500));
        if (millis() - t > 20000)
        {
            // Serial.println("Wifi Connect Fail!");
            menuWifiStatus.setTextValue("UnConnect");
            openDialog(TEXT_ERROR, TEXT_WIFI_CONN_FAIL_1, TEXT_WIFI_CONN_FAIL_2);

            delete[] wifiSSID;
            delete[] wifiPswd;
            vTaskDelete(NULL);
        }
    }

    // Serial.println("Wifi Connect Success!");
    menuWifiStatus.setTextValue("Connected");
    menuSSID.setTextValue(WiFi.SSID().c_str());
    menuIP.setIpAddress(WiFi.localIP().toString().c_str());

    delete[] wifiSSID;
    delete[] wifiPswd;
    vTaskDelete(NULL);
}

//------------------------------扫描Wifi信号
void scanWifi()
{
    SSIDCount = WiFi.scanNetworks();
    menuAllNetworks.setNumberOfRows(SSIDCount);
}

//------------------------------Wifi指令监听函数
void wifiCommand(void *pvParameters)
{
    byte wifiBuf[16] = {};
    int readByteCount = 0;
    unsigned long lastConn = millis();
    while (1)
    {
        WiFiClient client = server.available();
        readByteCount = 0;
        if (client)
        {
            while (client.connected())
            {
                if (client.available())
                {
                    wifiBuf[readByteCount] = client.read();
                    readByteCount += 1;
                    if (readByteCount > 15)
                    {
                        client.write(handleWifiCommand(wifiBuf, 16), 16);
                        lastConn = millis();
                    }
                }

                if (isSerialCommandEnabled())
                {
                    // 30秒发一个心跳包
                    if (millis() - lastConn > 30000)
                    {
                        client.write(HeartBeatPackage, 16);
                    }
                    // 5秒内无回应关闭通讯
                    if (millis() - lastConn > 35000)
                    {
                        disableWifiCommand();
                    }
                }
            }
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
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

// EEPROM中读取字符串
char *getStringEEPROM(int start_pos)
{
    char *s = new char[EEPROM.read(start_pos) + 1];
    s[EEPROM.read(start_pos)] = '\0';
    //    Serial.print("Read Length:");
    //    Serial.println(EEPROM.read(start_pos));
    for (int i = 0; i < EEPROM.read(start_pos); i++)
    {
        s[i] = char(EEPROM.read(start_pos + i + 1));
    }
    //    Serial.println(s);
    return s;
}

// 保存字符串至EEPROM
void writeStringEEPROM(int start_pos, int data_size, char *s)
{
    // Serial.println("length:");
    // Serial.println(strlen(s));
    EEPROM.write(start_pos, data_size);
    for (int i = 0; i < data_size; i++)
    {
        //        Serial.println(start_pos + i + 1);
        //        Serial.println(s[i]);
        EEPROM.write(start_pos + i + 1, s[i]);
    }
    EEPROM.commit();
}

//----------------------------------------
// 回调函数
//----------------------------------------
// 无线网络列表处理函数
int CALLBACK_FUNCTION fnAllNetworksRtCall(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize)
{
    switch (mode)
    {
    case RENDERFN_INVOKE:
        if (row < SSIDCount)
        {
            wifiSetConnection(row);
        }
        else if (row > 253)
        {
            scanWifi();
        }
        return true;
    case RENDERFN_NAME:
        if (row > 253)
        {
            strncpy(buffer, "wifi list", bufferSize);
        }
        else if (row <= SSIDCount)
        {
            WiFi.SSID(row).toCharArray(buffer, bufferSize);
        }
        return true;
    case RENDERFN_VALUE:
        if (row < SSIDCount)
        {
            strcpy(buffer, " ");
            fastltoa(buffer, WiFi.RSSI(row), 3, NOT_PADDED, bufferSize);
        }
        return true;
    case RENDERFN_EEPROM_POS:
        return 0xffff; // lists are generally not saved to EEPROM
    default:
        return false;
    }
}

// 文件列表处理函数
int CALLBACK_FUNCTION fnOpenFileRtCall(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize)
{
    switch (mode)
    {
    case RENDERFN_INVOKE:
        // TODO - your code to invoke goes here - row is the index of the item
        return true;
    case RENDERFN_NAME:
        // TODO - each row has it's own name - 0xff is the parent item
        // 254 and 255 are reserved for title and back, this is what is rendered in the back item text and also
        // as the list name / title on the parent menu.
        if (row > 253)
        {
            strncpy(buffer, "Open File", bufferSize);
        }
        else
        {
            strncpy(buffer, "Default", bufferSize);
        }
        return true;
    case RENDERFN_VALUE:
        // TODO - each row can has its own value - 0xff is the parent item
        // Set number of rows with .setNumberOfRows(int);
        if (row < 254)
        {
            strcpy(buffer, ">> ");
            fastltoa(buffer, row, 3, NOT_PADDED, bufferSize);
        }
        return true;
    case RENDERFN_EEPROM_POS:
        return 0xffff; // lists are generally not saved to EEPROM
    default:
        return false;
    }
}

void CALLBACK_FUNCTION onWifiSwitch(int id)
{
    saveValues(id);
    ESP.restart();
}

void CALLBACK_FUNCTION onChangeCurrent(int id)
{
    saveValues(id);
}

void CALLBACK_FUNCTION onChangeSpeed(int id)
{
    stepperX.setRPM(menuRunningSpeedX.getAsFloatingPointValue() / xPerimeter * 60);
    stepperY.setRPM(menuRunningSpeedY.getAsFloatingPointValue() / yPerimeter * 60);
    stepperZ.setRPM(menuRunningSpeedZ.getAsFloatingPointValue() / zPerimeter * 60);
    saveValues(id);
}

void CALLBACK_FUNCTION onChangeDiameter(int id)
{
    getStepperPerimeters();
    saveValues(id);
}

void CALLBACK_FUNCTION onChangeUseEncoder(int id)
{
    saveValues(id);
    ESP.restart();
}

void CALLBACK_FUNCTION calibrateEncoderCallback(int id)
{
    if (menuUseEncoderZ.getCurrentValue() == true)
    {
        xTaskCreate(calibrateEncoder,   //任务函数
                    "calibrateEncoder", //任务名称
                    8192,               //任务堆栈大小
                    NULL,               //任务参数
                    tskIDLE_PRIORITY,   //任务优先级
                    NULL);              //任务句柄
    }
    else
    {
        openDialog(TEXT_ERROR, TEXT_ENCODER_DISABLED, TEXT_EMPTY);
    }
}

void CALLBACK_FUNCTION resetSettings(int id)
{
    menuResetStatus.setBoolean(false);
    saveValues(id);
    ESP.restart();
}

// 设置网络菜单回调
void wifiSetConnection(int row)
{
    char buf[16];
    WiFi.SSID(row).toCharArray(buf, 16);
    menuSSIDToConnect.setTextValue(buf);
    if (WiFi.encryptionType(row) == 7)
    {
        menuPwd.setTextValue("");
        menuPwd.setVisible(false);
    }
    menuMgr.navigateToMenu(menuConnectTo.getChild());
}

// 尝试连接网络（设置后第一次连接）
void CALLBACK_FUNCTION wifiConnectAttempt(int id)
{
    openDialogNoButton(TEXT_ATTENTION, TEXT_CONNECTING, TEXT_PLEASE_WAIT);
    lastMenu = menuAllNetworks.getChildItem(0);

    for (int i = 300; i < 362; i++)
    {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();

    writeStringEEPROM(301, (int)strlen(menuSSIDToConnect.getTextValue()), (char *)menuSSIDToConnect.getTextValue());
    if (strlen(menuPwd.getTextValue()) == 0)
    {
        EEPROM.write(300, 1);
    }
    else
    {
        EEPROM.write(300, 2);
        writeStringEEPROM(331, (int)strlen(menuPwd.getTextValue()), (char *)menuPwd.getTextValue());
    }

    EEPROM.commit();

    //------------------------------重新创建连接Wifi任务
    // Serial.print(eTaskGetState(&wifiConnect_Handle));

    xTaskCreate(wifiConnect,          //任务函数
                "wifiConnect",        //任务名称
                8192,                 //任务堆栈大小
                NULL,                 //任务参数
                tskIDLE_PRIORITY,     //任务优先级
                &wifiConnect_Handle); //任务句柄
}

// 进纸
void CALLBACK_FUNCTION onChangeFeed(int id)
{
    if (menuFeed.getBoolean())
    {
        if (menuReverseDirection.getBoolean())
        {
            stepperZ.startMove(99999);
        }
        else
        {
            stepperZ.startMove(-99999);
        }
    }
    else
    {
        stepperZ.startMove(0);
        rotatedAngle = 0;
    }
}
