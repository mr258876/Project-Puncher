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
#include "PositionSensor/PositionSensor.h"
#include "StoreManager/StorageManager.h"
#include "PuncherUI.h"
#include "PowerManager/PowerManager.h"
#include "PuncherConf.h"

#include <Arduino.h>

// event group definitions
#define OnFinishX (1 << 0) // 0b1
#define OnFinishY (1 << 1) // 0b10
#define OnFinishZ (1 << 2) // 0b100

class PuncherScheduler : PuncherSchedulerInterface
{
private:
    /* Status codes*/
    puncher_status_t status;
    bool x_finished = 1;
    bool y_finished = 1;
    uint8_t y_status = 0; // 0->Idle, 1->Moved down, 2->Moved up
    bool z_finished = 1;

    /* Positions */
    double z_pos = 0;
    // by measurements, 14mm from base at top
    // 6.5mm from base at ready state
    // 3.5mm ftom base at full down
    double y_pos = Y_ZEROING_POSITION;
    // Count from positioning pillar on the right
    // 20.50mm base + 0.05 tape gap - 21mm cart
    // Hence it will be 0.45mm inside the tape
    // 5.55mm from the E3 line
    double x_pos = X_ZEROING_POSITION;

    double x_target_pos = 0;
    double y_target_pos = 0;
    double z_target_pos = 0;

    /* Task storage */
    std::vector<scheduler_hole_t> holeList;
    SemaphoreHandle_t holeListHandle;
    void handleMotorFinish();
    int nextHole();

    /* Motor related */
    MotorController *X = NULL;
    MotorController *Y = NULL;
    MotorController *Z = NULL;

    PositionSensor *sensor_X = NULL;
    PositionSensor *sensor_Y = NULL;
    PositionSensor *sensor_Z = NULL;

    bool sensor_X_avaliable = false;
    bool sensor_Y_avaliable = false;
    bool sensor_Z_avaliable = false;

    uint32_t calcMotorSpeedPulse(int32_t length10UM, uint16_t length_type, int32_t speed10UMpS, int32_t microSteps);
    uint32_t calcMotorSpeedPulse(std::any length10UM, std::any length_type, std::any speed10UMpS, std::any microSteps);

    void onFinishX();
    void onFinishY();
    void onFinishZ();

    inline void updateXspeed()
    {
        if (this->X)
            this->X->setSpeed(calcMotorSpeedPulse(this->x_lead_length, this->x_length_type, this->x_operational_speed, MICROSTEPS_X));
    }
    inline void updateYspeed()
    {
        if (this->Y)
            this->Y->setSpeed(calcMotorSpeedPulse(this->y_lead_length, this->y_length_type, this->y_operational_speed, MICROSTEPS_Y));
    }
    inline void updateZspeed()
    {
        if (this->Z)
            this->Z->setSpeed(calcMotorSpeedPulse(this->z_lead_length, this->z_length_type, this->z_operational_speed, MICROSTEPS_Z));
    }

    inline void updateXdriver()
    {
        if (this->X)
        {
            this->X->setReverse(std::any_cast<uint8_t>(this->x_reverse_axis));
            this->X->setCurrent(std::any_cast<int32_t>(this->x_operational_current));
        }
    }
    inline void updateYdriver()
    {
        if (this->Y)
        {
            this->Y->setReverse(std::any_cast<uint8_t>(this->y_reverse_axis));
            this->Y->setCurrent(std::any_cast<int32_t>(this->y_operational_current));
        }
    }
    inline void updateZdriver()
    {
        if (this->Z)
        {
            this->Z->setReverse(std::any_cast<uint8_t>(this->z_reverse_axis));
            this->Z->setCurrent(std::any_cast<int32_t>(this->z_operational_current));
        }
    }

    // Enter / Leave idle mode
    void Xsleep();
    void Xawake();
    void Ysleep();
    void Yawake();
    void Zsleep();
    void Zawake();

    inline int32_t calc_X_steps(double mm)
    {
        double steps = mm / (std::any_cast<int32_t>(x_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(x_length_type) ? 1 : 3.14159265358979) * MOTOR_STEPS * MICROSTEPS_X;
        steps += 0.5;
        return (int32_t)steps;
    }
    inline int32_t calc_Y_steps(double mm)
    {
        double steps = mm / (std::any_cast<int32_t>(y_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(y_length_type) ? 1 : 3.14159265358979) * MOTOR_STEPS * MICROSTEPS_Y;
        steps += 0.5;
        return (int32_t)steps;
    }
    inline int32_t calc_Z_steps(double mm)
    {
        double steps = mm / (std::any_cast<int32_t>(z_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(z_length_type) ? 1 : 3.14159265358979) * MOTOR_STEPS * MICROSTEPS_Z;
        steps = steps * (std::any_cast<int32_t>(z_cali_target_bar) * 8.0 / (std::any_cast<int32_t>(z_cali_measure_bar) * 8.0 + std::any_cast<int32_t>(z_cali_residual) / 1000.0));
        steps += 0.5;
        return (int32_t)steps;
    }

    /* user interfaces */
    std::vector<PuncherUI *> ui_list;

    /* Setting Values */
    std::any x_lead_length = std::any(static_cast<int32_t>(800));       // 8mm, last 2 digits for decimal
    std::any x_operational_speed = std::any(static_cast<int32_t>(800)); // 8mm, last 2 digits for decimal
    std::any x_length_type = std::any(static_cast<uint16_t>(0));        // 0->diameter 1->perimeter
    std::any x_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any x_operational_current = std::any(static_cast<int32_t>(1000));
    std::any x_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any x_sleep_current = std::any(static_cast<int32_t>(500));
    std::any x_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any x_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any x_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any x_zeroing_speed = std::any(static_cast<int32_t>(1000));
    std::any x_zeroing_position = std::any(static_cast<int32_t>(X_ZEROING_POSITION*100));

    std::any y_lead_length = std::any(static_cast<int32_t>(800));
    std::any y_operational_speed = std::any(static_cast<int32_t>(800));
    std::any y_length_type = std::any(static_cast<uint16_t>(0)); // 0->diameter 1->perimeter
    std::any y_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any y_operational_current = std::any(static_cast<int32_t>(1000));
    std::any y_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any y_sleep_current = std::any(static_cast<int32_t>(500));
    std::any y_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any y_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any y_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any y_zeroing_speed = std::any(static_cast<int32_t>(1000));
    std::any y_zeroing_position = std::any(static_cast<int32_t>(Y_ZEROING_POSITION*100));
    std::any y_punch_depth = std::any(static_cast<int32_t>(Y_PUNCH_MOVEMENT_LENGTH*100));

    std::any z_lead_length = std::any(static_cast<int32_t>(2000));
    std::any z_operational_speed = std::any(static_cast<int32_t>(1000));
    std::any z_length_type = std::any(static_cast<uint16_t>(0)); // 0->diameter 1->perimeter
    std::any z_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any z_operational_current = std::any(static_cast<int32_t>(1000));
    std::any z_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any z_sleep_current = std::any(static_cast<int32_t>(500));
    std::any z_cali_target_bar = std::any(static_cast<int32_t>(50));
    std::any z_cali_measure_bar = std::any(static_cast<int32_t>(50));
    std::any z_cali_residual = std::any(static_cast<int32_t>(0));
    std::any z_encoder_enable = std::any(static_cast<uint8_t>(0));
    std::any z_encoder_type = std::any(static_cast<uint16_t>(0)); // 0->AS5600

    std::any power_voltage = std::any(static_cast<uint16_t>(0)); // 0->5V 1->9V 2->12V 3->15V 4->20V

    std::any display_brightness = std::any(static_cast<int32_t>(128));
    std::any display_language = std::any(static_cast<uint16_t>(0));

    std::any mcode_default_tick_rate = std::any(static_cast<int32_t>(96));

    struct puncher_setting_mapping_t
    {
        puncher_setting_mapping_t() : obj(_any) {}
        puncher_setting_mapping_t(std::any &obj, puncher_storage_type_t val_type, std::function<bool(std::any)> cb) : obj(obj), type(val_type), call_back(cb) {}

        std::any &obj;
        puncher_storage_type_t type;
        std::function<bool(std::any)> call_back;

        std::any _any;
    };

    std::unordered_map<std::string, puncher_setting_mapping_t> setting_mapping;

    /* NVS storage */
    PuncherStorageManager *storage;
    void loadSettings();
    void initSettings();
    void saveValue(std::string name, puncher_setting_mapping_t item);

    /* event loop task & event queue */
    EventGroupHandle_t motor_evt_group;
    // QueueHandle_t evt_queue;
    friend void evtHandleLoop(void *param);

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

        ESP_LOGI("PuncherScheduler", "X driver status: %d", X->pingDriver());
        ESP_LOGI("PuncherScheduler", "Y driver status: %d", Y->pingDriver());
        ESP_LOGI("PuncherScheduler", "Z driver status: %d", Z->pingDriver());

        updateXspeed();
        updateYspeed();
        updateZspeed();

        updateXdriver();
        updateYdriver();
        updateZdriver();

        this->X->setMoveFinishCallBack([this]()
                                       { BaseType_t xHigherPriorityTaskWoken; xEventGroupSetBitsFromISR(this->motor_evt_group, OnFinishX, &xHigherPriorityTaskWoken); });
        this->Y->setMoveFinishCallBack([this]()
                                       { BaseType_t xHigherPriorityTaskWoken; xEventGroupSetBitsFromISR(this->motor_evt_group, OnFinishY, &xHigherPriorityTaskWoken); });
        this->Z->setMoveFinishCallBack([this]()
                                       { BaseType_t xHigherPriorityTaskWoken; xEventGroupSetBitsFromISR(this->motor_evt_group, OnFinishZ, &xHigherPriorityTaskWoken); });

        Xsleep();
        Ysleep();
        Zsleep();
    }

    inline void attachPositionSensors(PositionSensor *sensor_X, PositionSensor *sensor_Y, PositionSensor *sensor_Z)
    {
        this->sensor_X = sensor_X;
        this->sensor_Y = sensor_Y;
        this->sensor_Z = sensor_Z;

        if (sensor_Z)
        {
            sensor_Z_avaliable = (sensor_Z->ping() == 0);
        }
    }

    /* Attach UI before begin() ! */
    inline void attachUI(PuncherUI *ui)
    {
        this->ui_list.push_back(ui);
        ui->onStatusCode(&status);
        ui->attachScheduler(this);
    }

    /* update status to UI */
    inline void updateUIstatus()
    {
        for (PuncherUI *p_ui : this->ui_list)
        {
            p_ui->onStatusCode(&status);
        }
    }

    /* From PuncherSchedulerInterface */
    int start_workload();
    int pause_workload();
    int delete_workload();
    inline int data_transmit_mode(bool transmit_mode)
    {
        if (status.basic_status.status_data & (~(PUNCHER_STATUS_IS_TRANSMITTING_DATA | PUNCHER_STATUS_IS_FEEDING_PAPER)))
            return 1;

        status.basic_status.status_flags.is_transmitting_data = transmit_mode;

        if (transmit_mode)
        {
            holeList.clear();
        }
        else
        {
            status.task_length = holeList.size();
            status.finished_length = 0;
            if (status.task_length > 0)
                status.basic_status.status_flags.has_mission = 1;
        }

        updateUIstatus();
        return 0;
    }
    int feed_paper_mode(bool feed_paper_mode)
    {
        if (status.basic_status.status_data & (~(PUNCHER_STATUS_IS_FEEDING_PAPER | PUNCHER_STATUS_IS_TRANSMITTING_DATA)))
            return 1;

        status.basic_status.status_flags.is_feeding_paper = feed_paper_mode;
        if (feed_paper_mode)
        {
            Zawake();
            Z->sleep(true);
        }
        else
        {
            Z->sleep(false);
            Zsleep();
            if (sensor_Z) sensor_Z->clearRelativePosition();
        }

        return 0;
    }
    int add_hole(scheduler_hole_t &h);
    int add_hold_mcode(int x, int y, int z);
    int feed_paper(int gear);
    unsigned int set_status(unsigned int status_code);
    unsigned int get_status();
    time_t get_ETA();
    void set_setting_value(puncher_event_setting_change_t *evt);
    void get_setting_values(void *ui);
    inline void get_status(void *p_ui)
    {
        PuncherUI *ui = (PuncherUI *)p_ui;
        ui->onStatusCode(&status);
    }

    /* setters */
    bool setXLeadLength(std::any val);
    bool setXOperationalSpeed(std::any val);
    bool setXLengthType(std::any val);
    bool setXReverseAxis(std::any val);
    bool setXOperationalCurrent(std::any val);
    bool setXIdleBehavior(std::any val);
    bool setXSleepCurrent(std::any val);
    bool setXAutoZreoing(std::any val);
    bool setXZeroingTorchThres(std::any val);
    bool setXZeroingCurrent(std::any val);
    bool setXZeroingSpeed(std::any val);
    bool setXZeroingPosition(std::any val);

    bool setYLeadLength(std::any val);
    bool setYOperationalSpeed(std::any val);
    bool setYLengthType(std::any val);
    bool setYReverseAxis(std::any val);
    bool setYOperationalCurrent(std::any val);
    bool setYIdleBehavior(std::any val);
    bool setYSleepCurrent(std::any val);
    bool setYAutoZreoing(std::any val);
    bool setYZeroingTorchThres(std::any val);
    bool setYZeroingCurrent(std::any val);
    bool setYZeroingSpeed(std::any val);
    bool setYZeroingPosition(std::any val);
    bool setYPunchDepth(std::any val);

    bool setZLeadLength(std::any val);
    bool setZOperationalSpeed(std::any val);
    bool setZLengthType(std::any val);
    bool setZReverseAxis(std::any val);
    bool setZOperationalCurrent(std::any val);
    bool setZIdleBehavior(std::any val);
    bool setZSleepCurrent(std::any val);
    bool setZCaliTargetBar(std::any val);
    bool setZCaliMeasureBar(std::any val);
    bool setZCaliResidual(std::any val);
    bool setZEncoderEnable(std::any val);
    bool setZEncoderType(std::any val);

    bool setPowerVoltage(std::any option);

    bool setDisplayBrightness(std::any option);
    bool setDisplayLanguage(std::any option);

    bool setMcodeDefaultTickRate(std::any val);

    /* getters */
    inline uint16_t getDisplayLanguage() { return std::any_cast<uint16_t>(display_language); }
};

#endif
