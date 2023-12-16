#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include <stddef.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <any>
#include <string>

#include "PuncherSchedulerAbst.h"
#include "MotorController/MotorController.h"
#include "StoreManager/StorageManager.h"
#include "PuncherUI.h"

#include <Arduino.h>

class PuncherScheduler : PuncherSchedulerInterface
{
private:
    puncher_status_t status;

    /* Task storage */
    std::vector<scheduler_hole_t> holeList;
    SemaphoreHandle_t holeListHandle;

    MotorController *X;
    MotorController *Y;
    MotorController *Z;

    std::vector<PuncherUI *> ui_list;

    /* Setting Values */
    std::any x_lead_length = 2000;       // last 2 digits for decimal
    std::any x_operational_speed = 2000; // last 2 digits for decimal
    std::any x_reverse_axis = 0;
    std::any x_operational_current = 1000;
    std::any x_idle_behavior = 0;
    std::any x_sleep_current = 500;
    std::any x_auto_zreoing = 0;
    std::any x_zeroing_torch_thres = 128;
    std::any x_zeroing_current = 500;
    std::any x_zeroing_speed = 1000;

    std::any y_lead_length = 800;
    std::any y_operational_speed = 800;
    std::any y_reverse_axis = 0;
    std::any y_operational_current = 1000;
    std::any y_idle_behavior = 0;
    std::any y_sleep_current = 500;
    std::any y_auto_zreoing = 0;
    std::any y_zeroing_torch_thres = 128;
    std::any y_zeroing_current = 500;
    std::any y_zeroing_speed = 1000;

    std::any z_lead_length = 2000;
    std::any z_operational_speed = 2000;
    std::any z_reverse_axis = 0;
    std::any z_operational_current = 1000;
    std::any z_idle_behavior = 0;
    std::any z_sleep_current = 500;

    std::any power_voltage = 4;

    std::any display_brightness = 128;
    std::any display_language = 0;

    struct puncher_setting_mapping_t
    {
        puncher_setting_mapping_t() : obj(_any) {}
        puncher_setting_mapping_t(const char *name, std::any &obj, puncher_storage_type_t val_type, std::function<void(std::any)> cb) : obj_name(name), obj(obj), type(val_type), call_back(cb) {}

        std::string obj_name;
        std::any &obj;
        puncher_storage_type_t type;
        std::function<void(std::any)> call_back;

        std::any _any;
    };

    std::vector<puncher_setting_mapping_t> setting_mapping;

    /* NVS storage */
    PuncherStorageManager *storage;
    void loadSettings();
    void initSettings();
    void saveValue(puncher_setting_mapping_t item);

public:
    PuncherScheduler();
    ~PuncherScheduler();

    void begin();

    /* Attach motors before begin() ! */
    inline void attachMotors(MotorController *X, MotorController *Y, MotorController *Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
    }

    /* Attach UI before begin() ! */
    inline void attachUI(PuncherUI *ui)
    {
        this->ui_list.push_back(ui);
        ui->attachScheduler(this);
    }

    /* Notify all UI */
    inline void notifyUI(puncher_event_t *msg)
    {
        for (PuncherUI *p_ui : this->ui_list)
        {
            p_ui->onEvent(msg);
        }
    }

    inline void tick()
    {
        // if (status & PUNCHER_RUNNING)
        // motorHandler();
    }

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
    void set_setting_value(puncher_event_setting_change_t *evt);
};

#endif
