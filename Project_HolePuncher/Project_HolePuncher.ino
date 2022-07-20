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

unsigned stepperX_to_wait;
unsigned stepperY_to_wait;
unsigned stepperZ_to_wait;

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

    checkReset(); // 检查是否需要设置初始值

    menuProgress.setTextValue("idle");
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

    digitalWrite(xenablePin, LOW); // 启用x方向电机
    digitalWrite(yenablePin, LOW); // 启用y方向电机
    digitalWrite(zenablePin, LOW); // 启用z方向电机

    stepperX.begin(menuRunningSpeedX.getCurrentValue() * 60 / menuPerimeterX.getCurrentValue(), MICROSTEPS);
    stepperX.setEnableActiveState(LOW);
    stepperY.begin(menuRunningSpeedY.getCurrentValue() * 60 / menuPerimeterY.getCurrentValue(), MICROSTEPS);
    stepperY.setEnableActiveState(LOW);
    stepperZ.begin(menuRunningSpeedZ.getCurrentValue() * 60 / menuPerimeterZ.getCurrentValue(), MICROSTEPS);
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
    if (menuVirtualEndstopX.getCurrentValue())
    {
        // 中断方式
        // driverX.SGTHRS(menuEndstopThresholdX.getCurrentValue());
        // attachInterrupt(digitalPinToInterrupt(diagXPin), FUNCTION_PTR, MODE);
        uint8_t sgThrs = menuEndstopThresholdX.getCurrentValue();

        openDialogNoButton(TEXT_ATTENTION, TEXT_X_AXIS, TEXT_ZEROING);

        stepperX.startMove(99999);
        while (1)
        {
            stepperX.nextAction();
            if (driverX.SGTHRS() > sgThrs)
            {
                break;
            }
        }
    }

    if (menuVirtualEndstopY.getCurrentValue())
    {
        uint8_t sgThrs = menuEndstopThresholdY.getCurrentValue();

        openDialogNoButton(TEXT_ATTENTION, TEXT_Y_AXIS, TEXT_ZEROING);

        stepperY.startMove(99999);
        while (1)
        {
            stepperY.nextAction();
            if (driverY.SGTHRS() > sgThrs)
            {
                break;
            }
        }
        moveYto(8);
    }

    // Serial.println("Puncher booted.");
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

void checkReset()
{
    if (!menuResetStatus.getCurrentValue())
    {
        menuPerimeterX.setCurrentValue(80);
        menuPerimeterY.setCurrentValue(80);
        menuPerimeterZ.setCurrentValue(200);

        menuRunningSpeedX.setCurrentValue(80);
        menuRunningSpeedX.setCurrentValue(80);
        menuRunningSpeedX.setCurrentValue(200);

        menuRunningCurrentX.setCurrentValue(500);
        menuRunningCurrentY.setCurrentValue(800);
        menuRunningCurrentZ.setCurrentValue(500);

        menuVirtualEndstopX.setBoolean(false);
        menuVirtualEndstopY.setBoolean(false);

        menuUseEncoderZ.setBoolean(false);

        menuWifi.setBoolean(false);

        menuResetStatus.setBoolean(true);
        saveValues(0);
    }
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
                Serial.write(handleSerialCommand(serBuf, 8), 8);
            }
            xSemaphoreGive(Serial0Mutex);
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
            isPunching = true;
            Hole h = holeList.shift(); // 获取一个孔
            moveXpos(h.x);             // 移动X轴
            moveZ(h.z);                // 移动Z轴

            (String(holeFinished) + "/" + String(holeCount)).toCharArray(progressCA, 9);
            menuProgress.setTextValue(progressCA);
            menuETA.setTime(secToTime(calcETA(menuRunningSpeedX.getCurrentValue() * 60 / menuPerimeterX.getCurrentValue(), menuRunningSpeedY.getCurrentValue() * 60 / menuPerimeterY.getCurrentValue(), menuRunningSpeedZ.getCurrentValue() * 60 / menuPerimeterZ.getCurrentValue())));
            menuETA.setChanged(true);
        }

        if (Xenabled)
        {
            if (stepperX_to_wait <= 0)
            {
                Xenabled = false;
                startYaxis();
            }
        }

        if (Zenabled)
        {
            if (stepperZ_to_wait <= 0)
            {
                Zenabled = false;
                startYaxis();
            }
        }

        if (Yenabled)
        {
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
        if (encoderDisabled || abs(rotatedAngle / (4096.0 * menuPeriRatio.getCurrentValue() / 1000) * MOTOR_STEPS * MICROSTEPS + zLastMove) < 1)
        {
            moveYto(4);
            Ydown = true;
        }
        else
        {
            Zenabled = true;
            stepperZ.startMove((long)(zLastMove + rotatedAngle / (4096.0 * menuPeriRatio.getCurrentValue() / 1000) * MOTOR_STEPS * MICROSTEPS));
        }
    }
}

void moveZ(float mm)
{
    Zenabled = true;
    //     driverZ.rms_current(zOptiCurr);
    long toMove = (long)(-mm / menuPerimeterZ.getCurrentValue() * 10 * MOTOR_STEPS * MICROSTEPS);
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
    long steps = (long)(mm / menuPerimeterX.getCurrentValue() * 10 * (MOTOR_STEPS * MICROSTEPS) + 0.5);
    long toMove = xPosition - steps;
    stepperX.startMove(toMove);
    //    driverX.rms_current(xStbyCurr);
    xPosition = steps;
}

void moveYto(float mm)
{
    Yenabled = true;
    //    driverY.rms_current(yOptiCurr);
    long steps = (long)(mm / menuPerimeterY.getCurrentValue() * 10 * (MOTOR_STEPS * MICROSTEPS) + 0.5);
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
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// 编码器校准
void calibrateEncoder(void *pvParameters)
{
    if (puncherStatus == 0x10)
    {
        vTaskSuspend(runEncoder_Handle);    // 暂停编码器任务
        vTaskSuspend(punchSchedule_Handle); // 暂停调度任务

        uint16_t angleReading[MOTOR_STEPS * 2];
        uint16_t lastAngle = encoderZ.getAngle();
        uint16_t readingA = 0;
        uint16_t readingB = 0;
        // 正反转各一圈获取读数
        for (int i = 0; i < MOTOR_STEPS; i++)
        {
            stepperZ.startMove(MICROSTEPS);
            while (stepperZ_to_wait > 0)
            {
            }
            angleReading[i] = encoderZ.getAngle();
            vTaskDelay(pdMS_TO_TICKS(100));

            //            Serial.println(angleReading[i] - lastAngle);
            if (abs(angleReading[i] - lastAngle) > 2048)
            {
                readingA += 4096 - abs(angleReading[i] - lastAngle);
            }
            else
            {
                readingA += abs(angleReading[i] - lastAngle);
            }
            lastAngle = angleReading[i];
        }
        for (int i = 0; i < MOTOR_STEPS; i++)
        {
            stepperZ.startMove(-MICROSTEPS);
            while (stepperZ_to_wait > 0)
            {
            }
            angleReading[MOTOR_STEPS + i] = encoderZ.getAngle();
            vTaskDelay(pdMS_TO_TICKS(100));

            //            Serial.println(angleReading[i] - lastAngle);
            if (abs(angleReading[i] - lastAngle) > 2048)
            {
                readingB += 4096 - abs(angleReading[i] - lastAngle);
            }
            else
            {
                readingB += abs(angleReading[i] - lastAngle);
            }
            lastAngle = angleReading[i];
        }
        rotatedAngle = 0;

        menuPeriRatio.setCurrentValue((readingA + readingB) / 2 / 4096 * 1000);

        // 重启编码器任务
        vTaskResume(runEncoder_Handle);
        vTaskResume(punchSchedule_Handle);
    }
    else
    {
        openDialog(TEXT_ERROR, TEXT_PUNCHER_BUSY, TEXT_EMPTY);
    }

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
    byte wifiBuf[8] = {};
    int readByteCount = 0;
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
                    if (readByteCount > 7)
                    {
                        client.write(handleWifiCommand(wifiBuf, 8), 8);
                    }
                }
            }
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
    Serial.println("length:");
    Serial.println(strlen(s));
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
}

void CALLBACK_FUNCTION onChangeCurrent(int id)
{
    saveValues(id);
}

void CALLBACK_FUNCTION onChangeSpeed(int id)
{
    stepperX.setRPM(menuRunningSpeedX.getCurrentValue() * 60 / menuPerimeterX.getCurrentValue());
    stepperY.setRPM(menuRunningSpeedY.getCurrentValue() * 60 / menuPerimeterY.getCurrentValue());
    stepperZ.setRPM(menuRunningSpeedZ.getCurrentValue() * 60 / menuPerimeterZ.getCurrentValue());
    saveValues(id);
}

void CALLBACK_FUNCTION onChangePerimeter(int id)
{
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
    Serial.print(eTaskGetState(&wifiConnect_Handle));

    xTaskCreate(wifiConnect,          //任务函数
                "wifiConnect",        //任务名称
                8192,                 //任务堆栈大小
                NULL,                 //任务参数
                tskIDLE_PRIORITY,     //任务优先级
                &wifiConnect_Handle); //任务句柄
}
