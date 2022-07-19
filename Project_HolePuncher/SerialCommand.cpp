#include "SerialCommand.h"

byte msg[8];
CRC8 crc;

bool wifiCommandEnabled = true;

byte *response = new char[8];
byte *lastResponse = new char[8];

// 将response中内容复制一份随后返回
byte *returnAndCopy()
{
    memcpy(lastResponse, response, 8);
    return response;
}

byte *handleSerialCommand(byte *buffer, int len)
{
    if (!checkCRC(buffer, len))
    {
        memcpy(response, CRCErrorResp, 8);
        return response;
    }

    switch (buffer[0])
    {
    case 0xC0:
        return handle_Command(buffer, len);
    case 0xD0:
        return handle_DataReceive(buffer, len);
    case 0xEF:
        return handle_Resend();
    default:
        return handle_UnknownCommand();
    }
}

byte *handleWifiCommand(byte *buffer, int len)
{
    if (!wifiCommandEnabled)
    {
        memcpy(response, WifiCommandDisabledResp, 8);
        return response;
    }

    if (!checkCRC(buffer, len))
    {
        memcpy(response, CRCErrorResp, 8);
        return response;
    }
    return nullptr;
}

byte *handle_Resend()
{
    return lastResponse;
}

bool checkCRC(byte *buffer, int len)
{
    crc.reset();
    crc.add((uint8_t *)buffer, 7);
    return buffer[7] == (byte)crc.getCRC();
}

byte *handle_Command(byte *buffer, int len)
{
    switch (buffer[1])
    {
    case 0x01:
        return handle_StatusQuery();
    case 0x10:
        return handle_StartPunch();
    case 0xA0:
        return handle_StartDataTrans();
    case 0xA1:
        return handle_EndDataTrans();
    default:
        return handle_UnknownCommand();
    }
}

byte *handle_UnknownCommand()
{
    memcpy(response, UnknownCommandResp, 8);
    return returnAndCopy();
}

byte *handle_StatusQuery()
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

    memcpy(response, msg, 8);
    return returnAndCopy();
}

byte *handle_StartDataTrans()
{
    if (puncherStatus == 0x10)
    {
        puncherStatus = 0x01;
    }
    return handle_StatusQuery();
}

byte *handle_EndDataTrans()
{
    if (puncherStatus == 0x01)
    {
        puncherStatus = 0x10;
    }
    return handle_StatusQuery();
}

byte *handle_DataReceive(byte *buffer, int len)
{
    if (puncherStatus != 0x01)
    {
        return handle_DataTransDisabled();
    }

    if (buffer[1] > 29)
    {
        memcpy(response, DataXOutOfRangeResp, 8);
        return returnAndCopy();
    }

    FLOATUNION_t zPos;
    for (int i = 0; i < 4; i++)
    {
        zPos.floatBytes[i] = buffer[i + 2];
    }

    holeList.add(Hole{(int)buffer[1], zPos.number});

    memcpy(response, ClientSuccessResp, 8);
    return returnAndCopy();
}

byte *handle_DataTransDisabled()
{
    memcpy(response, DataTransDisabledResp, 8);
    return returnAndCopy();
}

byte *handle_StartPunch()
{
    if (puncherStatus == 0x10)
    {
        if (holeList.size() == 0)
        {
            memcpy(response, NoHoleDataResp, 8);
            return returnAndCopy();
        }
        puncherStatus = 0x11;
        punchStartTime = millis();
        holeFinished = 0;
        holeCount = holeList.size();
    }
    return handle_StatusQuery();
}
