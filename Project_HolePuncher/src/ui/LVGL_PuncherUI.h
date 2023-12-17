#ifndef _LVGL_PUNCHERUI_H_
#define _LVGL_PUNCHERUI_H_

#include "PuncherUI.h"
#include "FreeRTOS.h"
#include "freertos/queue.h"
#include <lvgl.h>

class LVGLPuncherUI : public PuncherUI
{
private:
    PuncherSchedulerInterface *scheduler = NULL;

    /* event loop task & event queue */
    QueueHandle_t evt_queue;
    friend void evtQueueHandleLoop(void *param);
    void handleEvent(puncher_event_t *msg);
public:
    lv_disp_drv_t disp_drv;
    lv_disp_draw_buf_t draw_buf;
    lv_color_t *disp_draw_buf;

    LVGLPuncherUI();
    ~LVGLPuncherUI();

    void begin();
    void onEvent(puncher_event_t *msg);
    void attachScheduler(PuncherSchedulerInterface *p_scheduler);
    inline PuncherSchedulerInterface *getScheduler() { return scheduler; }

    void onSettingValueChange(puncher_event_setting_change_t *msg);
    void setBrightness(int brightness);
};

extern LVGLPuncherUI *lvgl_ui;

#endif  // _LVGL_PUNCHERUI_H_