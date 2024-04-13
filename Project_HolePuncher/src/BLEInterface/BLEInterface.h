#ifndef _BLEINTERFACE_H_
#define _BLEINTERFACE_H_

#include "PuncherUI.h"
#include <NimBLEDevice.h>

class BLEInterface : public PuncherUI
{
private:
    /* status flag */
    uint8_t is_running = 0;
    void start();
    void stop();

    PuncherSchedulerInterface *scheduler = nullptr;
    NimBLEServer *pBLEServer = nullptr;
    NimBLEAdvertising *pBLEAdvertising = nullptr;

    void mcode_setup();
public:
    BLEInterface(/* args */);
    ~BLEInterface();

    void begin();

    /* From PuncherUI */
    void onStatusCode(puncher_status_t *data);
    void onSettingValueChange(puncher_event_setting_change_t *data);
    void attachScheduler(PuncherSchedulerInterface *p_scheduler);

    inline PuncherSchedulerInterface *getScheduler() { return scheduler; }
};

extern BLEInterface *bleInterface;

#endif