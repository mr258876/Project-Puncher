/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#ifndef MENU_GENERATED_CODE_H
#define MENU_GENERATED_CODE_H

#include <Arduino.h>
#include <tcMenu.h>
#include "tcMenuU8g2.h"
#include <RuntimeMenuItem.h>
#include <IoAbstraction.h>
#include <ArduinoEEPROMAbstraction.h>

// variables we declare that you may need to access
extern const PROGMEM ConnectorLocalInfo applicationInfo;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C gfx;
extern GraphicsDeviceRenderer renderer;

// Any externals needed by IO expanders, EEPROMs etc


// Global Menu Item exports
extern ActionMenuItem menuInfoNo;
extern ActionMenuItem menuInfoYes;
extern TextMenuItem menuInfo2;
extern TextMenuItem menuInfo1;
extern BackMenuItem menuBackInfoView;
extern SubMenuItem menuInfoView;
extern TextMenuItem menuAbout3;
extern TextMenuItem menuAbout2;
extern TextMenuItem menuAbout1;
extern BackMenuItem menuBackAbout;
extern SubMenuItem menuAbout;
extern BooleanMenuItem menuResetStatus;
extern ActionMenuItem menuResetConfirm;
extern TextMenuItem menuResetWarn2;
extern TextMenuItem menuResetWarn1;
extern BackMenuItem menuBackResetSettings;
extern SubMenuItem menuResetSettings;
extern BackMenuItem menuBackWireless;
extern SubMenuItem menuWireless;
extern BooleanMenuItem menuCalibrateEncoderZ;
extern BooleanMenuItem menuReversedEncoderZ;
extern BooleanMenuItem menuUseEncoderZ;
extern AnalogMenuItem menuRunningCurrentZ;
extern AnalogMenuItem menuRunningSpeedZ;
extern AnalogMenuItem menuPerimeterZ;
extern BackMenuItem menuBackZAxis;
extern SubMenuItem menuZAxis;
extern AnalogMenuItem menuRunningCurrentY;
extern AnalogMenuItem menuEndstopThresholdY;
extern BooleanMenuItem menuVirtualEndstopY;
extern AnalogMenuItem menuRunningSpeedY;
extern AnalogMenuItem menuPerimeterY;
extern BackMenuItem menuBackYAxis;
extern SubMenuItem menuYAxis;
extern AnalogMenuItem menuRunningCurrentX;
extern AnalogMenuItem menuEndstopThresholdX;
extern BooleanMenuItem menuVitrualEndstopX;
extern AnalogMenuItem menuRunningSpeedX;
extern AnalogMenuItem menuPerimeterX;
extern BackMenuItem menuBackXAxis;
extern SubMenuItem menuXAxis;
extern EnumMenuItem menuLanguage;
extern BackMenuItem menuBackSettings;
extern SubMenuItem menuSettings;
extern ListRuntimeMenuItem menuOpenFile;
extern BackMenuItem menuBackMenu;
extern SubMenuItem menuMenu;
extern TimeFormattedMenuItem menuDuration;
extern TimeFormattedMenuItem menuETA;
extern TextMenuItem menuProgress;

// Provide a wrapper to get hold of the root menu item and export setupMenu
inline MenuItem& rootMenuItem() { return menuProgress; }
void setupMenu();

// Callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

int fnOpenFileRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize);
void CALLBACK_FUNCTION resetSettings(int id);
void CALLBACK_FUNCTION updateLanguage(int id);

void openDialog(int title, int info1, int info2);
void openConfirmDialog(int title, int info1, int info2, MenuCallbackFn confirm_callback);
void openConfirmDialog(int title, int info1, int info2, MenuCallbackFn confirm_callback, MenuCallbackFn cancel_callback);
void openConfirmDialog(int title, int info1, int info2, int confirm, int cancel, bool show_confirm, bool show_cancel, MenuCallbackFn confirm_callback, MenuCallbackFn cancel_callback);

#endif // MENU_GENERATED_CODE_H
