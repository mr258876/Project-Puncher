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
#include "PowerManager/PowerManager.h"

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
    std::any x_lead_length = std::any(static_cast<int32_t>(800));       // 8mm, last 2 digits for decimal
    std::any x_operational_speed = std::any(static_cast<int32_t>(800)); // 8mm, last 2 digits for decimal
    std::any x_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any x_operational_current = std::any(static_cast<int32_t>(1000));
    std::any x_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any x_sleep_current = std::any(static_cast<int32_t>(500));
    std::any x_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any x_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any x_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any x_zeroing_speed = std::any(static_cast<int32_t>(1000));

    std::any y_lead_length = std::any(static_cast<int32_t>(800));
    std::any y_operational_speed = std::any(static_cast<int32_t>(800));
    std::any y_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any y_operational_current = std::any(static_cast<int32_t>(1000));
    std::any y_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any y_sleep_current = std::any(static_cast<int32_t>(500));
    std::any y_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any y_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any y_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any y_zeroing_speed = std::any(static_cast<int32_t>(1000));

    std::any z_lead_length = std::any(static_cast<int32_t>(2000));
    std::any z_operational_speed = std::any(static_cast<int32_t>(2000));
    std::any z_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any z_operational_current = std::any(static_cast<int32_t>(1000));
    std::any z_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any z_sleep_current = std::any(static_cast<int32_t>(500));

    std::any power_voltage = std::any(static_cast<uint16_t>(3));    // 15V

    std::any display_brightness = std::any(static_cast<int32_t>(128));
    std::any display_language = std::any(static_cast<uint16_t>(4));

    struct puncher_setting_mapping_t
    {
        puncher_setting_mapping_t() : obj(_any) {}
        puncher_setting_mapping_t(std::any &obj, puncher_storage_type_t val_type, std::function<void(std::any)> cb) : obj(obj), type(val_type), call_back(cb) {}

        std::any &obj;
        puncher_storage_type_t type;
        std::function<void(std::any)> call_back;

        std::any _any;
    };

    std::unordered_map<std::string, puncher_setting_mapping_t> setting_mapping;

    /* NVS storage */
    PuncherStorageManager *storage;
    void loadSettings();
    void initSettings();
    void saveValue(std::string name, puncher_setting_mapping_t item);

    // /* event loop task & event queue */
    // QueueHandle_t evt_queue;
    // friend void evtQueueHandleLoop(void *param);

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
    void get_setting_values(void *ui);

    /* setters */
    void setPowerVoltage(std::any option);
};

#endif
