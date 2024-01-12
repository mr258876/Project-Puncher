#ifndef _LVGL_PUNCHERUI_H_
#define _LVGL_PUNCHERUI_H_

#include "PuncherUI.h"
#include "FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include <lvgl.h>

#define OnEventCode (1 << 0)          // 0b1
#define OnSettingValueChange (1 << 1) // 0b10

class LVGLPuncherUI : public PuncherUI
{
private:
    PuncherSchedulerInterface *scheduler = NULL;

    /* event loop task & event queue */
    QueueHandle_t event_code_queue;
    QueueHandle_t setting_value_queue;
    EventGroupHandle_t evt_group;
    friend void evtQueueHandleLoop(void *param);
    friend void lvglLoop(void *param);
    uint16_t lang_id = 0;

    void handleEventCodeChange(puncher_status_t *msg);
    void handleSettingValueChange(puncher_event_setting_change_t *msg);

public:
    LVGLPuncherUI();
    ~LVGLPuncherUI();

    void begin();
    void onStatusCode(puncher_status_t *data);
    void onSettingValueChange(puncher_event_setting_change_t *msg);
    void attachScheduler(PuncherSchedulerInterface *p_scheduler);
    inline PuncherSchedulerInterface *getScheduler() { return scheduler; }

    void setBrightness(int brightness);
    void setLanguage(uint16_t lang_id);
};

extern LVGLPuncherUI *lvgl_ui;

#endif // _LVGL_PUNCHERUI_H_