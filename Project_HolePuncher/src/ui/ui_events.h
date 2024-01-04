#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

void ui_event_FeedRoller(lv_event_t *e);

void ui_event_mcodeTickRate(lv_event_t *e);

void ui_event_XLeadLength(lv_event_t *e);
void ui_event_XOperationalSpeed(lv_event_t *e);
void ui_event_XLengthType(lv_event_t *e);
void ui_event_XReverseAxis(lv_event_t *e);
void ui_event_XOperationalCurrent(lv_event_t *e);
void ui_event_XIdleBehavior(lv_event_t *e);
void ui_event_XSleepCurrent(lv_event_t *e);
void ui_event_XAutoZreoing(lv_event_t *e);
void ui_event_XZeroingTorchThres(lv_event_t *e);
void ui_event_XZeroingCurrent(lv_event_t *e);
void ui_event_XZeroingSpeed(lv_event_t *e);
void ui_event_XZeroingPosition(lv_event_t *e);

void ui_event_YLeadLength(lv_event_t *e);
void ui_event_YOperationalSpeed(lv_event_t *e);
void ui_event_YLengthType(lv_event_t *e);
void ui_event_YReverseAxis(lv_event_t *e);
void ui_event_YOperationalCurrent(lv_event_t *e);
void ui_event_YIdleBehavior(lv_event_t *e);
void ui_event_YSleepCurrent(lv_event_t *e);
void ui_event_YAutoZreoing(lv_event_t *e);
void ui_event_YZeroingTorchThres(lv_event_t *e);
void ui_event_YZeroingCurrent(lv_event_t *e);
void ui_event_YZeroingSpeed(lv_event_t *e);
void ui_event_YZeroingPosition(lv_event_t *e);
void ui_event_YPunchDepth(lv_event_t *e);

void ui_event_ZLeadLength(lv_event_t *e);
void ui_event_ZOperationalSpeed(lv_event_t *e);
void ui_event_ZLengthType(lv_event_t *e);
void ui_event_ZReverseAxis(lv_event_t *e);
void ui_event_ZOperationalCurrent(lv_event_t *e);
void ui_event_ZIdleBehavior(lv_event_t *e);
void ui_event_ZSleepCurrent(lv_event_t *e);
void ui_event_ZCaliTarget(lv_event_t *e);
void ui_event_ZCaliMeasure(lv_event_t *e);
void ui_event_ZCaliResidual(lv_event_t *e);

void ui_event_VoltageDropDown(lv_event_t * e);

void ui_event_BrightnessSlider(lv_event_t * e);
void ui_event_LanguageDropDown(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
