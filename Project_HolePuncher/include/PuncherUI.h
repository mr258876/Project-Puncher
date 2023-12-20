#ifndef _PUNCHERUI_H_
#define _PUNCHERUI_H_

#include "PuncherSchedulerAbst.h"
#include "PuncherEvents.h"

class PuncherUI
{
public:
    virtual void begin() = 0;
    /* For PuncherScheduler, call this to update values in UI, open a dialog, etc. */
    virtual void onStatusCode(puncher_event_code_t *data) = 0;
    virtual void onSettingValueChange(puncher_event_setting_change_t *data) = 0;
    /* For PuncherScheduler */
    virtual void attachScheduler(PuncherSchedulerInterface *p_scheduler) = 0;
};

#endif // _PUNCHERUI_H_