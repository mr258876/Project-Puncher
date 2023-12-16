#ifndef _PUNCHERUI_H_
#define _PUNCHERUI_H_


#include "PuncherSchedulerAbst.h"
#include "PuncherEvents.h"


class PuncherUI
{
private:
    PuncherSchedulerInterface *scheduler = NULL;
public:
    virtual void begin() = 0;
    /* For PuncherScheduler, call this to update values in UI, open a dialog, etc. */
     virtual void onEvent(puncher_event_t *data) = 0;
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