// #include "SerialCommand.h"

// byte msg[16];
// CRC8 crc;

// bool serialCommandEnabled = false;
// bool wifiCommandEnabled = false;

// byte *commResponse = new byte[16];
// byte *lastcommResponse = new byte[16];

// byte *HeartBeatPackage = (byte *)&HeartBeat;

// // 将commResponse中内容复制一份随后返回
// byte *returnAndCopy()
// {
//     memcpy(lastcommResponse, commResponse, 16);
//     return commResponse;
// }

// byte *handleSerialCommand(byte *buffer, int len)
// {
//     if (!checkCRC(buffer, len))
//     {
//         memcpy(commResponse, CRCErrorResp, 16);
//         return commResponse;
//     }

//     if (buffer[0] == 0xC0 && buffer[1] == 0x20)
//     {
//         serialCommandEnabled = true;
//         wifiCommandEnabled = false;
//         memcpy(commResponse, ClientSuccessResp, 16);
//         return commResponse;
//     }

//     if (!serialCommandEnabled)
//     {
//         memcpy(commResponse, SerialCommandDisabledResp, 16);
//         return commResponse;
//     }

//     switch (buffer[0])
//     {
//     case 0xC0:
//         return handle_Command(buffer, len);
//     case 0xD0:
//         return handle_DataReceive(buffer, len);
//     case 0xEF:
//         return handle_Resend();
//     default:
//         return handle_UnknownCommand();
//     }
// }

// byte *handleWifiCommand(byte *buffer, int len)
// {
//     if (!checkCRC(buffer, len))
//     {
//         memcpy(commResponse, CRCErrorResp, 16);
//         return commResponse;
//     }

//     if (buffer[0] == 0xC0 && buffer[1] == 0x21 && !serialCommandEnabled)
//     {
//         wifiCommandEnabled = true;
//         memcpy(commResponse, ClientSuccessResp, 16);
//         return commResponse;
//     }

//     if (!wifiCommandEnabled)
//     {
//         memcpy(commResponse, WifiCommandDisabledResp, 16);
//         return commResponse;
//     }

//     switch (buffer[0])
//     {
//     case 0xC0:
//         return handle_Command(buffer, len);
//     case 0xD0:
//         return handle_DataReceive(buffer, len);
//     case 0xEF:
//         return handle_Resend();
//     default:
//         return handle_UnknownCommand();
//     }
// }

// byte *handle_Resend()
// {
//     return lastcommResponse;
// }

// bool checkCRC(byte *buffer, int len)
// {
//     crc.reset();
//     crc.add((uint8_t *)buffer, 15);
//     return buffer[15] == (byte)crc.getCRC();
// }

// byte *handle_Command(byte *buffer, int len)
// {
//     switch (buffer[1])
//     {
//     case 0x01:
//         memcpy(commResponse, ClientSuccessResp, 16);
//         return commResponse;
//     case 0x10:
//         return handle_StatusQuery();
//     case 0x11:
//         return handle_StartPunch();
//     case 0x30:
//         return handle_SerialCommandDisable();
//     case 0x31:
//         return handle_WifiCommandDisable();
//     case 0xA0:
//         return handle_StartDataTrans();
//     case 0xA1:
//         return handle_EndDataTrans();
//     case 0xB0:
//         return handle_SetCursor(buffer, len);
//     default:
//         return handle_UnknownCommand();
//     }
// }

// byte *handle_SerialCommandDisable()
// {
//     serialCommandEnabled = false;
//     memcpy(commResponse, ClientSuccessResp, 16);
//     return commResponse;
// }

// byte *handle_WifiCommandDisable()
// {
//     wifiCommandEnabled = false;
//     memcpy(commResponse, ClientSuccessResp, 16);
//     return commResponse;
// }

// byte *handle_UnknownCommand()
// {
//     memcpy(commResponse, UnknownCommandResp, 16);
//     return returnAndCopy();
// }

// byte *handle_StatusQuery()
// {
//     crc.reset();
//     msg[0] = 0xE0;
//     msg[1] = 0x10;
//     msg[2] = puncherStatus;
//     for (int i = 3; i < 16; i++)
//     {
//         msg[i] = 0x00;
//     }
//     crc.add((uint8_t *)msg, 15);
//     msg[15] = (byte)crc.getCRC();

//     memcpy(commResponse, msg, 16);
//     return returnAndCopy();
// }

// byte *handle_StartDataTrans()
// {
//     if (puncherStatus == 0x10)
//     {
//         puncherStatus = 0x01;
//     }
//     return handle_StatusQuery();
// }

// byte *handle_EndDataTrans()
// {
//     if (puncherStatus == 0x01)
//     {
//         puncherStatus = 0x10;
//     }
//     return handle_StatusQuery();
// }

// byte *handle_SetCursor(byte *buffer, int len)
// {
//     // 设置当前打孔位置
//     // 注意：打孔位置从1开始计算
//     // 若设置当前打孔位置为x，则将从x处开始打孔(包含x)
//     if (puncherStatus != 0x10)
//     {
//         memcpy(commResponse, PuncherBusyResp, 16);
//         return returnAndCopy();
//     }
    
//     UINT16UNION_t hole_pos;
//     for (int i = 0; i < 2; i++)
//     {
//         hole_pos.uint16Bytes[i] = buffer[i + 2];
//     }

//     if (holeList.size() < hole_pos.number)
//     {
//         memcpy(commResponse, DataOutOfRangeResp, 16);
//         return returnAndCopy();
//     }

//     for (int i = 0; i < hole_pos.number-1; i++)
//     {
//         holeList.shift();
//     }
//     holeFinished = hole_pos.number-1;
//     lengthZ = holeList.getRoot()->data.z;

//     memcpy(commResponse, ClientSuccessResp, 16);
//     return returnAndCopy();
// }

// byte *handle_DataReceive(byte *buffer, int len)
// {
//     if (puncherStatus != 0x01)
//     {
//         return handle_DataTransDisabled();
//     }

//     if (buffer[1] > 29)
//     {
//         memcpy(commResponse, DataXOutOfRangeResp, 16);
//         return returnAndCopy();
//     }

//     DOUBLEUNION_t zPos;
//     for (int i = 0; i < 8; i++)
//     {
//         zPos.doubleBytes[i] = buffer[i + 2];
//     }

//     holeList.add(Hole{(int)buffer[1], zPos.number});

//     memcpy(commResponse, ClientSuccessResp, 16);
//     return returnAndCopy();
// }

// byte *handle_DataTransDisabled()
// {
//     memcpy(commResponse, DataTransDisabledResp, 16);
//     return returnAndCopy();
// }

// byte *handle_StartPunch()
// {
//     if (puncherStatus == 0x10)
//     {
//         if (holeList.size() == 0)
//         {
//             memcpy(commResponse, NoHoleDataResp, 16);
//             return returnAndCopy();
//         }
//         puncherStatus = 0x11;
//         punchStartTime = millis();
//         holeFinished = 0;
//         holeCount = holeList.size();
//     }
//     return handle_StatusQuery();
// }

// void disableSerialCommand()
// {
//     serialCommandEnabled = false;
// }

// void disableWifiCommand()
// {
//     wifiCommandEnabled = false;
// }

// bool isSerialCommandEnabled()
// {
//     return serialCommandEnabled;
// }

// bool isWifiCommandEnabled()
// {
//     return wifiCommandEnabled;
// }