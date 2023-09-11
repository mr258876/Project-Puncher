#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include <stddef.h>
#include <vector>
#include <algorithm>
#include <time.h>

#include "Observe.h"
#include "PuncherSchedulerAbst.h"
#include "MotorController/MotorController.h"
#include "PuncherUI.h"

#include <Arduino.h>

class PuncherScheduler : PuncherSchedulerInterface
{
private:
    puncher_status_t status;

    std::vector<scheduler_hole_t> holeList;
    SemaphoreHandle_t holeListHandle;

    MotorController *X;
    MotorController *Y;
    MotorController *Z;

    std::vector<PuncherUI *> ui_list;

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
        this->ui_list.push_back(ui);
        ui->addObserver([this](void *param)
                        { this->onUINotify(param); });
    }
    /* Notify all UI */
    inline void notifyUI(ui_alert_msg_t *msg)
    {
        for (PuncherUI *p_ui : this->ui_list)
        {
            p_ui->alert(msg);
        }
    }

    inline void tick()
    {
        // if (status & PUNCHER_RUNNING)
        // motorHandler();
    }

    void ui_cb_draw_configurable_menu(PuncherUI *ui, Configurable *item, void *ui_param);

    /* Observation callbacks */
    void onMotorNotify(void *param);
    void onUINotify(void *param);

    /* From PuncherSchedulerInterface */
    int start_workload();
    int pause_workload();
    int delete_workload();
    int add_hole(scheduler_hole_t h);
    int feed_paper(int gear);
    int ui_get_menu();
    unsigned int set_status(unsigned int status_code);
    unsigned int get_status();
    time_t get_ETA();
};

#endif
