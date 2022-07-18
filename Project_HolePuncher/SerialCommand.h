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

void handleSerialCommand(byte* buffer, int len);
void handle_Resend();
bool checkCRC(byte *buffer, int len);
void handle_Command(byte *buffer, int len);
void handle_UnknownCommand();
void handle_StatusQuery();
void handle_StartDataTrans();
void handle_EndDataTrans();
void handle_DataReceive(byte *buffer, int len);
void handle_DataTransDisabled();
void handle_StartPunch();

extern bool wifiCommandEnabled;
extern bool serialCommandEnabled;

extern char* response;

#endif
