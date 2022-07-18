/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <EEPROM.h>
#include <tcMenu.h>
#include "Project_HolePuncher_menu.h"
#include "ThemeMonoInverse.h"
#include "multiLanguage.h"
#include "MenuItems.h"

// Global variable declarations
const PROGMEM ConnectorLocalInfo applicationInfo = {"Proj.HolePuncher", "88eacbfd-af3c-4e0a-bee7-b7e9e9827f97"};
ArduinoEEPROMAbstraction glArduinoEeprom(&EEPROM);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C gfx(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
U8g2Drawable gfxDrawable(&gfx);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);

int language = 0;

int dialogTitle = TEXT_ATTENTION;
int dialogInfo1 = TEXT_ATTENTION;
int dialogInfo2 = TEXT_ATTENTION;

MenuItem *lastMenu;

// Global Menu Item declarations
AnyMenuInfo_ML minfoInfoNo(lang[language][TEXT_CANCEL], 5, 0xffff, 0, NO_CALLBACK);
ActionMenuItem menuInfoNo(&minfoInfoNo, NULL);
AnyMenuInfo_ML minfoInfoYes(lang[language][TEXT_CONFIRM], 4, 0xffff, 0, NO_CALLBACK);
ActionMenuItem menuInfoYes(&minfoInfoYes, &menuInfoNo);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnInfo2RtCall, textItemRenderFn, lang[language][dialogInfo2], -1, NO_CALLBACK)
TextMenuItem menuInfo2(fnInfo2RtCall, 3, 1, &menuInfoYes);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnInfo1RtCall, textItemRenderFn, lang[language][dialogInfo1], -1, NO_CALLBACK)
TextMenuItem menuInfo1(fnInfo1RtCall, 2, 1, &menuInfo2);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnInfoViewRtCall, textItemRenderFn, lang[language][dialogTitle], -1, NO_CALLBACK)
SubMenuInfo_ML minfoInfoView(lang[language][TEXT_ATTENTION], 1, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackInfoView(fnInfoViewRtCall, &menuInfo1);
SubMenuItem menuInfoView(&minfoInfoView, &menuBackInfoView, NULL);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnAbout3RtCall, textItemRenderFn, lang[language][TEXT_ABOUT_3], -1, NO_CALLBACK)
TextMenuItem menuAbout3(fnAbout3RtCall, 14, 1, NULL);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnAbout2RtCall, textItemRenderFn, lang[language][TEXT_ABOUT_2], -1, NO_CALLBACK)
TextMenuItem menuAbout2(fnAbout2RtCall, 13, 1, &menuAbout3);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnAbout1RtCall, textItemRenderFn, lang[language][TEXT_ABOUT_1], -1, NO_CALLBACK)
TextMenuItem menuAbout1(fnAbout1RtCall, 12, 1, &menuAbout2);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnAboutRtCall, backSubItemRenderFn, lang[language][TEXT_ABOUT], -1, NO_CALLBACK)
SubMenuInfo_ML minfoAbout(lang[language][TEXT_ABOUT], 11, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackAbout(fnAboutRtCall, &menuAbout1);
SubMenuItem menuAbout(&minfoAbout, &menuBackAbout, NULL);
BooleanMenuInfo_ML minfoResetStatus = {lang[language][TEXT_RESET_STATUS], 41, 3, 1, NO_CALLBACK, NAMING_TRUE_FALSE};
BooleanMenuItem menuResetStatus(&minfoResetStatus, false, NULL);
AnyMenuInfo_ML minfoResetConfirm = {lang[language][TEXT_CONFIRM], 40, 0xffff, 0, resetSettings};
ActionMenuItem menuResetConfirm(&minfoResetConfirm, &menuResetStatus);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnResetWarn2RtCall, textItemRenderFn, lang[language][TEXT_RESET_WARN_2], -1, NO_CALLBACK)
TextMenuItem menuResetWarn2(fnResetWarn2RtCall, 39, 5, &menuResetConfirm);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnResetWarn1RtCall, textItemRenderFn, lang[language][TEXT_RESET_WARN_1], -1, NO_CALLBACK)
TextMenuItem menuResetWarn1(fnResetWarn1RtCall, 38, 5, &menuResetWarn2);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnResetSettingsRtCall, backSubItemRenderFn, lang[language][TEXT_RESET_ALL], -1, NO_CALLBACK)
SubMenuInfo_ML minfoResetSettings = {lang[language][TEXT_RESET_ALL], 37, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackResetSettings(fnResetSettingsRtCall, &menuResetWarn1);
SubMenuItem menuResetSettings(&minfoResetSettings, &menuBackResetSettings, NULL);
AnyMenuInfo_ML minfoConnect = {lang[language][TEXT_CONNECT], 49, 0xffff, 0, wifiConnectAttempt};
ActionMenuItem menuConnect(&minfoConnect, NULL);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnPwdRtCall, textItemRenderFn, lang[language][TEXT_PASSWORD], -1, NO_CALLBACK)
TextMenuItem menuPwd(fnPwdRtCall, 48, 16, &menuConnect);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnSSIDToConnectRtCall, textItemRenderFn, lang[language][TEXT_SSID], -1, NO_CALLBACK)
TextMenuItem menuSSIDToConnect(fnSSIDToConnectRtCall, 47, 5, &menuPwd);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnConnectToRtCall, backSubItemRenderFn, lang[language][TEXT_CONNECT_TO], -1, NO_CALLBACK)
SubMenuInfo_ML minfoConnectTo = {lang[language][TEXT_CONNECT_TO], 46, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackConnectTo(fnConnectToRtCall, &menuSSIDToConnect);
SubMenuItem menuConnectTo(&minfoConnectTo, &menuBackConnectTo, NULL);
ListRuntimeMenuItem menuAllNetworks(45, 0, fnAllNetworksRtCall, &menuConnectTo);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnIPRtCall, ipAddressRenderFn, lang[language][TEXT_IP_ADDRESS], -1, NO_CALLBACK)
IpAddressMenuItem menuIP(fnIPRtCall, 44, &menuAllNetworks);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnSSIDRtCall, textItemRenderFn, lang[language][TEXT_SSID], -1, NO_CALLBACK)
TextMenuItem menuSSID(fnSSIDRtCall, 43, 10, &menuIP);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnWifiStatusRtCall, textItemRenderFn, lang[language][TEXT_WIFI_STATUS], -1, NO_CALLBACK)
TextMenuItem menuWifiStatus(fnWifiStatusRtCall, 51, 10, &menuSSID);
BooleanMenuInfo_ML minfoWifi = {lang[language][TEXT_AUTO_CONNECT], 42, 32, 1, onWifiSwitch, NAMING_ON_OFF};
BooleanMenuItem menuWifi(&minfoWifi, false, &menuWifiStatus);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnWirelessRtCall, backSubItemRenderFn, lang[language][TEXT_WIRELESS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoWireless = {lang[language][TEXT_WIRELESS], 36, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackWireless(fnWirelessRtCall, &menuWifi);
SubMenuItem menuWireless(&minfoWireless, &menuBackWireless, &menuResetSettings);
AnyMenuInfo_ML minfoCalibrateEncoderZ = {lang[language][TEXT_CALIBRATE_ENCODER], 50, 0xffff, 0, calibrateEncoder};
ActionMenuItem menuCalibrateEncoderZ(&minfoCalibrateEncoderZ, NULL);
BooleanMenuInfo_ML minfoReversedEncoderZ = {lang[language][TEXT_REVERSED_ENCODER], 30, 25, 1, saveValues, NAMING_TRUE_FALSE};
BooleanMenuItem menuReversedEncoderZ(&minfoReversedEncoderZ, false, &menuCalibrateEncoderZ);
BooleanMenuInfo_ML minfoUseEncoderZ = {lang[language][TEXT_USE_ENCODER], 29, 24, 1, onChangeUseEncoder, NAMING_TRUE_FALSE};
BooleanMenuItem menuUseEncoderZ(&minfoUseEncoderZ, false, &menuReversedEncoderZ);
AnalogMenuInfo_ML minfoRunningCurrentZ = {lang[language][TEXT_RUNNING_CURRENT], 33, 30, 255, onChangeCurrent, 0, 1, "mA"};
AnalogMenuItem menuRunningCurrentZ(&minfoRunningCurrentZ, 0, &menuUseEncoderZ);
AnalogMenuInfo_ML minfoRunningSpeedZ = {lang[language][TEXT_RUNNING_SPEED], 32, 28, 600, onChangeSpeed, 1, 1, "mm"};
AnalogMenuItem menuRunningSpeedZ(&minfoRunningSpeedZ, 0, &menuRunningCurrentZ);
AnalogMenuInfo_ML minfoPerimeterZ = {lang[language][TEXT_PERIMETER], 31, 26, 255, onChangePerimeter, 1, 1, "mm"};
AnalogMenuItem menuPerimeterZ(&minfoPerimeterZ, 0, &menuRunningSpeedZ);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnZAxisRtCall, backSubItemRenderFn, lang[language][TEXT_Z_AXIS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoZAxis = {lang[language][TEXT_Z_AXIS], 18, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackZAxis(fnZAxisRtCall, &menuPerimeterZ);
SubMenuItem menuZAxis(&minfoZAxis, &menuBackZAxis, &menuWireless);
AnalogMenuInfo_ML minfoEndstopThresholdY = {lang[language][TEXT_ENDSTOP_THRESHOLD], 25, 16, 255, saveValues, 0, 1, ""};
AnalogMenuItem menuEndstopThresholdY(&minfoEndstopThresholdY, 0, NULL);
BooleanMenuInfo_ML minfoVirtualEndstopY = {lang[language][TEXT_VIRTUAL_ENDSTOP], 24, 15, 1, saveValues, NAMING_TRUE_FALSE};
BooleanMenuItem menuVirtualEndstopY(&minfoVirtualEndstopY, false, &menuEndstopThresholdY);
AnalogMenuInfo_ML minfoRunningCurrentY = {lang[language][TEXT_RUNNING_CURRENT], 28, 22, 1023, onChangeCurrent, 0, 1, "mA"};
AnalogMenuItem menuRunningCurrentY(&minfoRunningCurrentY, 0, &menuVirtualEndstopY);
AnalogMenuInfo_ML minfoRunningSpeedY = {lang[language][TEXT_RUNNING_SPEED], 27, 20, 600, onChangeSpeed, 1, 1, "mm"};
AnalogMenuItem menuRunningSpeedY(&minfoRunningSpeedY, 0, &menuRunningCurrentY);
AnalogMenuInfo_ML minfoPerimeterY = {lang[language][TEXT_PERIMETER], 26, 18, 255, onChangePerimeter, 1, 1, "mm"};
AnalogMenuItem menuPerimeterY(&minfoPerimeterY, 0, &menuRunningSpeedY);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnYAxisRtCall, backSubItemRenderFn, lang[language][TEXT_Y_AXIS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoYAxis = {lang[language][TEXT_Y_AXIS], 17, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackYAxis(fnYAxisRtCall, &menuPerimeterY);
SubMenuItem menuYAxis(&minfoYAxis, &menuBackYAxis, &menuZAxis);
AnalogMenuInfo_ML minfoEndstopThresholdX = {lang[language][TEXT_ENDSTOP_THRESHOLD], 20, 7, 255, saveValues, 0, 1, ""};
AnalogMenuItem menuEndstopThresholdX(&minfoEndstopThresholdX, 0, NULL);
BooleanMenuInfo_ML minfoVirtualEndstopX = {lang[language][TEXT_VIRTUAL_ENDSTOP], 19, 6, 1, saveValues, NAMING_TRUE_FALSE};
BooleanMenuItem menuVirtualEndstopX(&minfoVirtualEndstopX, false, &menuEndstopThresholdX);
AnalogMenuInfo_ML minfoRunningCurrentX = {lang[language][TEXT_RUNNING_CURRENT], 23, 13, 1023, onChangeCurrent, 0, 1, "mA"};
AnalogMenuItem menuRunningCurrentX(&minfoRunningCurrentX, 0, &menuVirtualEndstopX);
AnalogMenuInfo_ML minfoRunningSpeedX = {lang[language][TEXT_RUNNING_SPEED], 22, 11, 600, onChangeSpeed, 1, 1, "mm"};
AnalogMenuItem menuRunningSpeedX(&minfoRunningSpeedX, 0, &menuRunningCurrentX);
AnalogMenuInfo_ML minfoPerimeterX = {lang[language][TEXT_PERIMETER], 21, 9, 255, onChangePerimeter, 1, 1, "mm"};
AnalogMenuItem menuPerimeterX(&minfoPerimeterX, 0, &menuRunningSpeedX);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnXAxisRtCall, backSubItemRenderFn, lang[language][TEXT_X_AXIS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoXAxis = {lang[language][TEXT_X_AXIS], 16, 0xffff, 0, NO_CALLBACK};
BackMenuItem menuBackXAxis(fnXAxisRtCall, &menuPerimeterX);
SubMenuItem menuXAxis(&minfoXAxis, &menuBackXAxis, &menuYAxis);
const char enumStrLanguage_0[] PROGMEM = "ENG";
const char enumStrLanguage_1[] PROGMEM = "CHS";
const char *const enumStrLanguage[] PROGMEM = {enumStrLanguage_0, enumStrLanguage_1};
EnumMenuInfo_ML minfoLanguage = {lang[language][TEXT_LANGUAGE], 15, 4, 1, updateLanguage, enumStrLanguage};
EnumMenuItem menuLanguage(&minfoLanguage, 0, &menuXAxis);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnSettingsRtCall, backSubItemRenderFn, lang[language][TEXT_SETTINGS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoSettings(lang[language][TEXT_SETTINGS], 10, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackSettings(fnSettingsRtCall, &menuLanguage);
SubMenuItem menuSettings(&minfoSettings, &menuBackSettings, &menuAbout);
ListRuntimeMenuItem menuOpenFile(34, 0, fnOpenFileRtCall, &menuSettings);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnMenuRtCall, backSubItemRenderFn, lang[language][TEXT_MENU], -1, NO_CALLBACK)
SubMenuInfo_ML minfoMenu(lang[language][TEXT_MENU], 9, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackMenu(fnMenuRtCall, &menuOpenFile);
SubMenuItem menuMenu(&minfoMenu, &menuBackMenu, &menuInfoView); // 变更：&menuInfoView
RENDERING_CALLBACK_NAME_INVOKE_ML(fnDurationRtCall, timeItemRenderFn, lang[language][TEXT_DURATION], -1, NO_CALLBACK)
TimeFormattedMenuItem menuDuration(fnDurationRtCall, 8, (MultiEditWireType)6, &menuMenu);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnETARtCall, timeItemRenderFn, lang[language][TEXT_REMAINING], -1, NO_CALLBACK)
TimeFormattedMenuItem menuETA(fnETARtCall, 7, (MultiEditWireType)2, &menuDuration);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnProgressRtCall, textItemRenderFn, lang[language][TEXT_PROGRESS], -1, NO_CALLBACK)
TextMenuItem menuProgress(fnProgressRtCall, 6, 9, &menuETA);

void setupMenu()
{
    // First we set up eeprom and authentication (if needed).
    EEPROM.begin(512);
    menuMgr.setEepromRef(&glArduinoEeprom);

    // Now add any readonly, non-remote and visible flags.
    menuProgress.setReadOnly(true);
    menuETA.setReadOnly(true);
    menuDuration.setReadOnly(true);
    menuInfo1.setReadOnly(true);
    menuInfo2.setReadOnly(true);
    menuAbout1.setReadOnly(true);
    menuAbout2.setReadOnly(true);
    menuAbout3.setReadOnly(true);
    menuResetWarn1.setReadOnly(true);
    menuResetWarn2.setReadOnly(true);
    menuWifiStatus.setReadOnly(true);
    menuSSID.setReadOnly(true);
    menuIP.setReadOnly(true);
    menuSSIDToConnect.setReadOnly(true);

    menuInfoView.setVisible(false);
    menuResetStatus.setVisible(false);
    menuConnectTo.setVisible(false);

    // Code generated by plugins.
    gfx.begin();
    renderer.setUpdatesPerSecond(10);
    switches.initialise(internalDigitalIo(), true);
    menuMgr.initForEncoder(&renderer, &menuProgress, 32, 34, 35);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(false);
    installMonoInverseTitleTheme(renderer, MenuFontDef(u8g2_font_wqy14_t_gb2312b, 1), MenuFontDef(u8g2_font_wqy14_t_gb2312b, 1), true);

    menuOpenFile.setNumberOfRows(1);

    menuMgr.load();
    updateLanguage(0);
}

void CALLBACK_FUNCTION saveValues(int id)
{
    menuMgr.save();
    EEPROM.commit();
}

void CALLBACK_FUNCTION updateLanguage(int id)
{
    language = menuLanguage.getCurrentValue();

    strcpy(minfoInfoView.name, lang[language][TEXT_ATTENTION]);
    strcpy(minfoInfoNo.name, lang[language][TEXT_CANCEL]);
    strcpy(minfoInfoYes.name, lang[language][TEXT_CONFIRM]);

    // 主菜单
    strcpy(minfoMenu.name, lang[language][TEXT_MENU]);
    strcpy(minfoAbout.name, lang[language][TEXT_ABOUT]);
    strcpy(minfoSettings.name, lang[language][TEXT_SETTINGS]);
    strcpy(minfoLanguage.name, lang[language][TEXT_LANGUAGE]);

    // Z轴菜单
    strcpy(minfoReversedEncoderZ.name, lang[language][TEXT_REVERSED_ENCODER]);
    strcpy(minfoUseEncoderZ.name, lang[language][TEXT_USE_ENCODER]);
    strcpy(minfoRunningCurrentZ.name, lang[language][TEXT_RUNNING_CURRENT]);
    strcpy(minfoRunningSpeedZ.name, lang[language][TEXT_RUNNING_SPEED]);
    strcpy(minfoPerimeterZ.name, lang[language][TEXT_PERIMETER]);
    strcpy(minfoZAxis.name, lang[language][TEXT_Z_AXIS]);
    strcpy(minfoCalibrateEncoderZ.name, lang[language][TEXT_CALIBRATE_ENCODER]);

    // Y轴菜单
    strcpy(minfoVirtualEndstopY.name, lang[language][TEXT_VIRTUAL_ENDSTOP]);
    strcpy(minfoEndstopThresholdY.name, lang[language][TEXT_ENDSTOP_THRESHOLD]);
    strcpy(minfoRunningCurrentY.name, lang[language][TEXT_RUNNING_CURRENT]);
    strcpy(minfoRunningSpeedY.name, lang[language][TEXT_RUNNING_SPEED]);
    strcpy(minfoPerimeterY.name, lang[language][TEXT_PERIMETER]);
    strcpy(minfoYAxis.name, lang[language][TEXT_Y_AXIS]);

    // X轴菜单
    strcpy(minfoVirtualEndstopX.name, lang[language][TEXT_VIRTUAL_ENDSTOP]);
    strcpy(minfoEndstopThresholdX.name, lang[language][TEXT_ENDSTOP_THRESHOLD]);
    strcpy(minfoRunningCurrentX.name, lang[language][TEXT_RUNNING_CURRENT]);
    strcpy(minfoRunningSpeedX.name, lang[language][TEXT_RUNNING_SPEED]);
    strcpy(minfoPerimeterX.name, lang[language][TEXT_PERIMETER]);
    strcpy(minfoXAxis.name, lang[language][TEXT_X_AXIS]);

    // 重置菜单
    strcpy(minfoResetSettings.name, lang[language][TEXT_RESET_ALL]);
    strcpy(minfoResetConfirm.name, lang[language][TEXT_CONFIRM]);
    strcpy(minfoResetStatus.name, lang[language][TEXT_RESET_STATUS]);

    // 无线设置菜单
    strcpy(minfoWireless.name, lang[language][TEXT_WIRELESS]);
    strcpy(minfoConnect.name, lang[language][TEXT_CONNECT]);
    strcpy(minfoConnectTo.name, lang[language][TEXT_CONNECT_TO]);
    strcpy(minfoWifi.name, lang[language][TEXT_AUTO_CONNECT]);

    saveValues(0);
}


void CALLBACK_FUNCTION toLastMenu(int id)
{
    menuMgr.navigateToMenu(lastMenu);
}

void openDialog(int title, int info1, int info2)
{
    openConfirmDialog(title, info1, info2, TEXT_CONFIRM, TEXT_CANCEL, true, false, toLastMenu, NULL);
}

void openDialogNoButton(int title, int info1, int info2)
{
    openConfirmDialog(title, info1, info2, TEXT_CONFIRM, TEXT_CANCEL, false, false, NULL, NULL);
}

void openConfirmDialog(int title, int info1, int info2, MenuCallbackFn confirm_callback)
{
    openConfirmDialog(title, info1, info2, confirm_callback, toLastMenu);
}

void openConfirmDialog(int title, int info1, int info2, MenuCallbackFn confirm_callback, MenuCallbackFn cancel_callback)
{
    openConfirmDialog(title, info1, info2, TEXT_CONFIRM, TEXT_CANCEL, true, true, confirm_callback, cancel_callback);
}

void openConfirmDialog(int title, int info1, int info2, int confirm, int cancel, bool show_confirm, bool show_cancel, MenuCallbackFn confirm_callback, MenuCallbackFn cancel_callback)
{
    lastMenu = menuMgr.getCurrentMenu();

    dialogTitle = title;
    dialogInfo1 = info1;
    dialogInfo2 = info2;

    if (show_cancel)
    {
        strcpy(minfoInfoNo.name, lang[language][cancel]);
    }
    if (show_confirm)
    {
        strcpy(minfoInfoYes.name, lang[language][confirm]);
    }

    menuInfoNo.setVisible(show_cancel);
    menuInfoYes.setVisible(show_confirm);

    minfoInfoNo.callback = cancel_callback;
    minfoInfoYes.callback = confirm_callback;

    menuMgr.navigateToMenu(menuInfoView.getChild());
}

void closeDialog()
{
    menuMgr.navigateToMenu(lastMenu);
}
