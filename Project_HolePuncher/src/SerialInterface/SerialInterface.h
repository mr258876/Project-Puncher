#ifndef _SERIALINTERFACE_H_
#define _SERIALINTERFACE_H_

#include "PuncherUI.h"

class SerialInterface: public PuncherUI
{
private:
    /* data */
public:
    SerialInterface(/* args */);
    ~SerialInterface();

    void begin();

    /* From PuncherUI */
    void onStatusCode(puncher_event_code_t *data);
    void onSettingValueChange(puncher_event_setting_change_t *data);
    void attachScheduler(PuncherSchedulerInterface *p_scheduler);
};


#endif  // _SERIALINTERFACE_H_