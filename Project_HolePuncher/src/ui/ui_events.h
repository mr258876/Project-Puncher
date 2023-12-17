#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

void ui_event_BrightnessSlider(lv_event_t * e);
void ui_event_VoltageDropDown(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
