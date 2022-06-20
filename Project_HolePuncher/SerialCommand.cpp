#include "SerialCommand.h"

byte msg[8];
byte *lastFrameSend;
CRC8 crc;

void handleSerialCommand(byte *buffer, int len)
{
    if (!checkCRC(buffer, len))
    {
        Serial.write(CRCErrorResp, 8);
        lastFrameSend = (byte *)CRCErrorResp;
        return;
    }
    
    switch (buffer[0])
    {
    case 0xC0:
        handle_Command(buffer, len);
        break;
    case 0xD0:
        handle_DataReceive(buffer, len);
        break;
    case 0xEF:
        handle_Resend();
        break;
    default:
        handle_UnknownCommand();
        break;
    }
}

void handle_Resend()
{
    Serial.write(lastFrameSend, 8);
}

bool checkCRC(byte *buffer, int len)
{   
    crc.reset();
    crc.add((uint8_t *)buffer, 7);
    return buffer[7] == (byte)crc.getCRC();
}

void handle_Command(byte *buffer, int len)
{
    switch (buffer[1])
    {
    case 0x01:
        handle_StatusQuery();
        break;
    case 0x10:
        handle_StartPunch();
        break;
    case 0xA0:
        handle_StartDataTrans();
        break;
    case 0xA1:
        handle_EndDataTrans();
        break;
    default:
        handle_UnknownCommand();
        break;
    }
}

void handle_UnknownCommand()
{
    Serial.write(UnknownCommandResp, 8);
    lastFrameSend = (byte *)UnknownCommandResp;
}

void handle_StatusQuery()
{
    crc.reset();
    msg[0] = 0xE0;
    msg[1] = 0x10;
    msg[2] = puncherStatus;
    for (int i = 3; i < 8; i++)
    {
        msg[i] = 0x00;
    }
    crc.add((uint8_t *)msg, 7);
    msg[7] = (byte)crc.getCRC();

    Serial.write(msg, 8);
    lastFrameSend = (byte *)msg;
}

void handle_StartDataTrans()
{
    if (puncherStatus == 0x10)
    {
        puncherStatus = 0x01;
    }
    handle_StatusQuery();
}

void handle_EndDataTrans()
{
    if (puncherStatus == 0x01)
    {
        puncherStatus = 0x10;
    }
    handle_StatusQuery();
}

void handle_DataReceive(byte *buffer, int len)
{
    if (puncherStatus != 0x01)
    {
        handle_DataTransDisabled();
        return;
    }

    if (buffer[1] > 29)
    {
        Serial.write(DataXOutOfRangeResp, 8);
        lastFrameSend = (byte *)DataXOutOfRangeResp;
        return;
    }

    FLOATUNION_t zPos;
    for (int i = 0; i < 4; i++)
    {
        zPos.floatBytes[i] = buffer[i + 2];
    }

    holeList.add(Hole{(int)buffer[1], zPos.number});

    Serial.write(ClientSuccessResp, 8);
    lastFrameSend = (byte *)ClientSuccessResp;
}

void handle_DataTransDisabled()
{
    Serial.write(DataTransDisabledResp, 8);
    lastFrameSend = (byte *)DataTransDisabledResp;
}

void handle_StartPunch()
{
    if (puncherStatus == 0x10)
    {
        if (holeList.size() == 0)
        {   
            Serial.write(NoHoleDataResp, 8);
            lastFrameSend = (byte *)NoHoleDataResp;
            return;
        }
        puncherStatus = 0x11;
        punchStartTime = millis();
        holeFinished = 0;
        holeCount = holeList.size();
    }
    handle_StatusQuery();
}
