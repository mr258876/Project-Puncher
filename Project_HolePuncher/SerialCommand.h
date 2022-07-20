#ifndef _SERIAL_COMMAND_H_
#define _SERIAL_COMMAND_H_

#include <Arduino.h>
#include "PuncherScheduler.h"
#include <CRC8.h>

typedef union
{
    float number;
    byte floatBytes[4];
} FLOATUNION_t;

const PROGMEM byte ClientSuccessResp[8] = {0xE0, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x25};
const PROGMEM byte DataTransDisabledResp[8] = {0xE0, 0x10, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xC0};
const PROGMEM byte NoHoleDataResp[8] = {0xE0, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xAD};
const PROGMEM byte UnknownCommandResp[8] = {0xE0, 0xE0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC2};
const PROGMEM byte DataXOutOfRangeResp[8] = {0xE0, 0xE1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x84};
const PROGMEM byte EncoderErrorResp[8] = {0xE0, 0xE2, 0x02, 0x00, 0x00, 0x00, 0x00, 0x59};
const PROGMEM byte ZJamErrorResp[8] = {0xE0, 0xE2, 0x03, 0x00, 0x00, 0x00, 0x00, 0x3B};
const PROGMEM byte CRCErrorResp[8] = {0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97};
// The following CRC byte needs recalc.
const PROGMEM byte PuncherBusyResp[8] = {0xE0, 0xE0, 0x11, 0x00, 0x00, 0x00, 0x00, 0xD9};
const PROGMEM byte SerialCommandDisabledResp[8] = {0xE0, 0xE0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xB9};
const PROGMEM byte WifiCommandDisabledResp[8] = {0xE0, 0xE0, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xB9};
const PROGMEM byte EncoderNotDetectedResp[8] = {0xE0, 0xE2, 0x04, 0x00, 0x00, 0x00, 0x00, 0x73};
const PROGMEM byte HeartBeat[8] = {0xE0, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x75};

extern byte* HeartBeatPackage;

byte *handleSerialCommand(byte *buffer, int len);
byte *handleWifiCommand(byte *buffer, int len);
byte *handle_Resend();
bool checkCRC(byte *buffer, int len);
byte *handle_Command(byte *buffer, int len);
byte *handle_UnknownCommand();
byte *handle_StatusQuery();
byte *handle_StartDataTrans();
byte *handle_EndDataTrans();
byte *handle_DataReceive(byte *buffer, int len);
byte *handle_DataTransDisabled();
byte *handle_StartPunch();
byte *handle_SerialCommandDisable();
byte *handle_WifiCommandDisable();

void disableSerialCommand();
void disableWifiCommand();
bool isSerialCommandEnabled();
bool isWifiCommandEnabled();

#endif
