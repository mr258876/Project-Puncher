#ifndef _SERIALINTERFACE_H_
#define _SERIALINTERFACE_H_

#include "PuncherUI.h"

class SerialInterface: public PuncherUI
{
private:
    PuncherSchedulerInterface *scheduler;
public:
    SerialInterface(/* args */);
    ~SerialInterface();

    void begin();
    void enableLog();

    /* From PuncherUI */
    void onStatusCode(puncher_status_t *data);
    void onSettingValueChange(puncher_event_setting_change_t *data);
    void attachScheduler(PuncherSchedulerInterface *p_scheduler);

    inline PuncherSchedulerInterface *getScheduler() { return scheduler; }
};

extern SerialInterface *usbInterface;

#endif  // _SERIALINTERFACE_H_