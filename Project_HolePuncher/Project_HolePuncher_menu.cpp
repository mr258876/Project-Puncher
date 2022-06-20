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

// Global variable declarations
const PROGMEM ConnectorLocalInfo applicationInfo = {"Proj.HolePuncher", "88eacbfd-af3c-4e0a-bee7-b7e9e9827f97"};
ArduinoEEPROMAbstraction glArduinoEeprom(&EEPROM);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C gfx(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
U8g2Drawable gfxDrawable(&gfx);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);

int language = 0;

// Global Menu Item declarations
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
const char enumStrLanguage_0[] PROGMEM = "English";
const char enumStrLanguage_1[] PROGMEM = "Chinese";
const char* const enumStrLanguage[] PROGMEM  = { enumStrLanguage_0, enumStrLanguage_1 };
EnumMenuInfo_ML minfoLanguage = {lang[language][TEXT_LANGUAGE], 15, 4, 1, updateLanguage, enumStrLanguage };
EnumMenuItem menuLanguage(&minfoLanguage, 0, NULL);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnSettingsRtCall, backSubItemRenderFn, lang[language][TEXT_SETTINGS], -1, NO_CALLBACK)
SubMenuInfo_ML minfoSettings(lang[language][TEXT_SETTINGS], 10, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackSettings(fnSettingsRtCall, &menuLanguage);
SubMenuItem menuSettings(&minfoSettings, &menuBackSettings, &menuAbout);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnMenuRtCall, backSubItemRenderFn, lang[language][TEXT_MENU], -1, NO_CALLBACK)
SubMenuInfo_ML minfoMenu(lang[language][TEXT_MENU], 9, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackMenu(fnMenuRtCall, &menuSettings);
SubMenuItem menuMenu(&minfoMenu, &menuBackMenu, NULL);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnDurationRtCall, timeItemRenderFn, lang[language][TEXT_DURATION], -1, NO_CALLBACK)
TimeFormattedMenuItem menuDuration(fnDurationRtCall, 8, (MultiEditWireType)6, &menuMenu);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnETARtCall, timeItemRenderFn, lang[language][TEXT_REMAINING], -1, NO_CALLBACK)
TimeFormattedMenuItem menuETA(fnETARtCall, 7, (MultiEditWireType)2, &menuDuration);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnProgressRtCall, textItemRenderFn, lang[language][TEXT_PROGRESS], -1, NO_CALLBACK)
TextMenuItem menuProgress(fnProgressRtCall, 6, 9, &menuETA);
AnyMenuInfo_ML minfoInfoNo(lang[language][TEXT_CANCEL], 5, 0xffff, 0, NO_CALLBACK);
ActionMenuItem menuInfoNo(&minfoInfoNo, NULL);
AnyMenuInfo_ML minfoInfoYes(lang[language][TEXT_CONFIRM], 4, 0xffff, 0, NO_CALLBACK);
ActionMenuItem menuInfoYes(&minfoInfoYes, &menuInfoNo);
RENDERING_CALLBACK_NAME_INVOKE(fnInfo2RtCall, textItemRenderFn, "Info2", -1, NO_CALLBACK)
TextMenuItem menuInfo2(fnInfo2RtCall, 3, 1, &menuInfoYes);
RENDERING_CALLBACK_NAME_INVOKE(fnInfo1RtCall, textItemRenderFn, "Info1", -1, NO_CALLBACK)
TextMenuItem menuInfo1(fnInfo1RtCall, 2, 1, &menuInfo2);
RENDERING_CALLBACK_NAME_INVOKE_ML(fnInfoViewRtCall, backSubItemRenderFn, lang[language][TEXT_ATTENTION], -1, NO_CALLBACK)
SubMenuInfo_ML minfoInfoView(lang[language][TEXT_ATTENTION], 1, 0xffff, 0, NO_CALLBACK);
BackMenuItem menuBackInfoView(fnInfoViewRtCall, &menuInfo1);
SubMenuItem menuInfoView(&minfoInfoView, &menuBackInfoView, &menuProgress);

void setupMenu()
{
    // First we set up eeprom and authentication (if needed).
    EEPROM.begin(512);
    menuMgr.setEepromRef(&glArduinoEeprom);
    
    // Now add any readonly, non-remote and visible flags.
    menuAbout3.setReadOnly(true);
    menuDuration.setReadOnly(true);
    menuAbout2.setReadOnly(true);
    menuAbout1.setReadOnly(true);
    menuProgress.setReadOnly(true);
    menuETA.setReadOnly(true);
    menuInfoView.setVisible(false);

    // Code generated by plugins.
    gfx.begin();
    renderer.setUpdatesPerSecond(10);
    switches.init(internalDigitalIo(), SWITCHES_POLL_EVERYTHING, true);
    menuMgr.initForEncoder(&renderer, &menuInfoView, 32, 34, 35);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(false);
    installMonoInverseTitleTheme(renderer, MenuFontDef(u8g2_font_wqy14_t_gb2312b, 1), MenuFontDef(u8g2_font_wqy14_t_gb2312b, 1), true);

    menuMgr.load();
    updateLanguage(0);
}

void CALLBACK_FUNCTION updateLanguage(int id) {
    language = menuLanguage.getCurrentValue();
    
    strcpy(minfoInfoView.name, lang[language][TEXT_ATTENTION]);
    strcpy(minfoInfoNo.name, lang[language][TEXT_CANCEL]);
    strcpy(minfoInfoYes.name, lang[language][TEXT_CONFIRM]);
    
    strcpy(minfoMenu.name, lang[language][TEXT_MENU]);
    strcpy(minfoAbout.name, lang[language][TEXT_ABOUT]);
    
    strcpy(minfoSettings.name, lang[language][TEXT_SETTINGS]);
    strcpy(minfoLanguage.name, lang[language][TEXT_LANGUAGE]);

    menuMgr.save();
    EEPROM.commit();
    
}
