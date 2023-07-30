#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include <stddef.h>
#include <vector>
#include <algorithm>
#include <time.h>

#include "Observe.h"
#include "MotorController/MotorController.h"
#include "PuncherUI.h"
#include "PuncherSchedulerTyping.h"

#include <Arduino.h>

class PuncherScheduler
{
private:
    puncher_status_t status;

    std::vector<scheduler_hole_t> holeList;

    MotorController *X;
    MotorController *Y;
    MotorController *Z;

    PuncherUI *ui;

    void motorHandler();

    void prepareMenu();

public:
    PuncherScheduler();
    ~PuncherScheduler();

    void begin();

    /* Attach motors before begin() ! */
    inline void attachMotors(MotorController *X, MotorController *Y, MotorController *Z)
    {
        this->X = X;
        X->addObserver([this](void *param)
                       { this->onMotorNotify(param); });
        this->Y = Y;
        Y->addObserver([this](void *param)
                       { this->onMotorNotify(param); });
        this->Z = Z;
        Z->addObserver([this](void *param)
                       { this->onMotorNotify(param); });
    }
    /* Attach UI before begin() ! */
    inline void attachUI(PuncherUI *ui)
    {
        this->ui = ui;
        ui->addObserver([this](void *param)
                       { this->onUINotify(param); });
    }

    inline bool addHole(scheduler_hole_t h)
    {
        holeList.push_back(h);
        return true;
    }

    time_t getETA();
    inline bool startPunch()
    {
        if (status)
            return false;

        reverse(holeList.begin(), holeList.end()); // Reverse the vector so it will be O(1) each time poping a hole

        // TODO
        status = PUNCHER_RUNNING;
        return true;
    }

    inline void tick()
    {
        // if (status & PUNCHER_RUNNING)
            // motorHandler();
    }


    void ui_cb_draw_configurable_menu(PuncherUI *ui, Configurable *item, void* ui_param);


    /* Observation callbacks */
    void onMotorNotify(void *param);
    void onUINotify(void *param);
};

#endif
