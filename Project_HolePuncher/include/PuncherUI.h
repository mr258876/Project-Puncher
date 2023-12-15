#ifndef _PUNCHERUI_H_
#define _PUNCHERUI_H_

#include "Observe.h"
#include "ObserveStructs.h"
#include "PuncherSchedulerAbst.h"
#include <string>

enum ui_menu_item_type_t
{
    MENU_ITEM_TYPE_DEFAULT = 0,
    MENU_ITEM_TYPE_SUB_MENU,
    MENU_ITEM_TYPE_CALLBACK,
    MENU_ITEM_TYPE_TEXT,
    MENU_ITEM_TYPE_QR_CODE,
    MENU_ITEM_TYPE_CHECKBOX,
    MENU_ITEM_TYPE_SWITCH,
    MENU_ITEM_TYPE_SPINBOX,
    MENU_ITEM_TYPE_SLIDER,
    MENU_ITEM_TYPE_DROP_DOWN,
};

struct __packed ui_menu_item_t
{
    using ui_menu_item_cb_t = std::function<int(void *)>;
    const char* disp_name;
    ui_menu_item_type_t item_type;
    ui_menu_item_cb_t cb;
    void *data;
};

struct __packed ui_alert_msg_t
{
    void *title;
    void *msg;
    void *button_text;
    void *button_cb;
};

class PuncherUI
{
private:
    PuncherSchedulerInterface *scheduler = NULL;
public:
    virtual void begin() = 0;
    /* For PuncherScheduler, call this to update values in UI, open a dialog, etc. */
    virtual void alert(ui_alert_msg_t *msg) = 0;
    /* For PuncherScheduler, call this to draw menu. */
    virtual void drawMenu(std::vector<ui_menu_item_t>, void* ui_param=NULL) = 0;
    /* For PuncherScheduler */
    inline void attachScheduler(PuncherSchedulerInterface *p_scheduler)
    {
        this->scheduler = p_scheduler;
    }
    inline PuncherSchedulerInterface *getScheduler()
    {
        return scheduler;
    }
};

#endif // _PUNCHERUI_H_