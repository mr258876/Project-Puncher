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

class PuncherScheduler : PuncherSchedulerInterface
{
private:
    /* init flag */
    bool has_begin = false;

    /* Status codes*/
    puncher_status_t status;
    inline bool status_has_error() { return status.basic_status.status_data & PUNCHER_STATUS_ERROR_CHECK; }
    bool x_finished = 1;
    bool y_finished = 1;
    uint8_t y_status = 0; // 0->Idle, 1->Moved down, 2->Moved up
    bool z_finished = 1;

    /* Positions */
    double _z_pos = 0;
    // by measurements, 14mm from base at top
    // 6.5mm from base at ready state
    // 3.5mm ftom base at full down
    double _y_pos = Y_ZEROING_POSITION;
    // Count from positioning pillar on the right
    // 20.50mm base + 0.05 tape gap - 21mm cart
    // Hence it will be 0.45mm inside the tape
    // 5.55mm from the E3 line
    double _x_pos = X_ZEROING_POSITION;

    double _x_target_pos = 0;
    double _y_target_pos = 0;
    double _z_target_pos = 0;

    bool x_zeroed = false;
    bool y_zeroed = false;
    bool z_zeroed = false;

    /* Task storage */
    std::vector<scheduler_hole_t> holeList;
    SemaphoreHandle_t holeListHandle;
    void handleMotorFinish();
    int nextHole();

    int start_workload_cb();
    int pause_workload_cb();
    int resume_workload_cb();
    int delete_workload_cb();

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

    bool sensor_X_initialized = false;
    bool sensor_Y_initialized = false;
    bool sensor_Z_initialized = false;

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

    void onFinishZeroingX();
    void onFinishZeroingY();
    void onFinishZeroingZ();

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
        if (mm > 0)
            steps += 0.5;
        else if (mm < 0)
            steps -= 0.5;
        return (int32_t)steps;
    }
    inline int32_t calc_Y_steps(double mm)
    {
        double steps = mm / (std::any_cast<int32_t>(y_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(y_length_type) ? 1 : 3.14159265358979) * MOTOR_STEPS * MICROSTEPS_Y;
        if (mm > 0)
            steps += 0.5;
        else if (mm < 0)
            steps -= 0.5;
        return (int32_t)steps;
    }
    inline int32_t calc_Z_steps(double mm)
    {
        double steps = mm / (std::any_cast<int32_t>(z_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(z_length_type) ? 1 : 3.14159265358979) * MOTOR_STEPS * MICROSTEPS_Z;
        steps = steps * (std::any_cast<int32_t>(z_cali_target_bar) * 8.0 / (std::any_cast<int32_t>(z_cali_measure_bar) * 8.0 + std::any_cast<int32_t>(z_cali_residual) / 1000.0));
        if (mm > 0)
            steps += 0.5;
        else if (mm < 0)
            steps -= 0.5;
        return (int32_t)steps;
    }

    static void _X_on_finish_move_static(void *arg);
    static void _Y_on_finish_move_static(void *arg);
    static void _Z_on_finish_move_static(void *arg);

    void _X_on_finish_move();
    void _Y_on_finish_move();
    void _Z_on_finish_move();

    void initMotors();

    int _util_move_X_dir;
    bool _util_move_X_zeroing_conf;
    int util_move_X_cb();
    int _util_move_Y_dir;
    bool _util_move_Y_zeroing_conf;
    int util_move_Y_cb();

    int read_sg_result_X_cb();
    int read_sg_result_Y_cb();
    int read_sg_result_Z_cb();

    int start_auto_zeroing_X_cb();
    int start_auto_zeroing_Y_cb();

    /* init seneors */
    inline void _initSensorZ()
    {
        if (this->sensor_Z)
        {
            if (!this->sensor_Z_initialized)
            {
                this->sensor_Z->begin();
                this->sensor_Z_initialized = true;
            }
            sensor_Z_avaliable = (this->sensor_Z->ping() == 0);
        }
    }
    inline void initSensors()
    {
        _initSensorZ();
    }

    int start_sensor_calibration_Z_cb();
    void onCalibratingZ();

    /* user interfaces */
    std::vector<PuncherUI *> ui_list;
    void notifyValueChange(puncher_event_setting_change_t *evt)
    {
        /* Notify all UI after value change */
        for (auto &ui : this->ui_list)
        {
            ui->onSettingValueChange(evt);
        }
    }

    inline void initUI()
    {
        for (auto ui : ui_list)
        {
            ui->onStatusCode(&status);
            ui->attachScheduler(this);
        }
    }

    /* power manager*/
    PowerManager *pm;
    inline void initPower()
    {
        pm->set_pgood_cb([this](uint8_t res)
                         { this->status.basic_status.status_flags.power_err = res; scheduler_evt_t evt = EVT_ON_POWER_STATUS_CHANGE; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });
        pm->acquire_voltage(std::any_cast<uint16_t>(power_voltage));
    }
    void onPowerStatusChange();

    /* Setting Values */
    std::any x_lead_length = std::any(static_cast<int32_t>(800));       // 8mm, last 2 digits for decimal
    std::any x_operational_speed = std::any(static_cast<int32_t>(800)); // 8mm, last 2 digits for decimal
    std::any x_length_type = std::any(static_cast<uint16_t>(0));        // 0->diameter 1->perimeter
    std::any x_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any x_operational_current = std::any(static_cast<int32_t>(1000));
    std::any x_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any x_sleep_current = std::any(static_cast<int32_t>(500));
    std::any x_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any x_zeroing_reverse_dir = std::any(static_cast<uint8_t>(0));
    std::any x_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any x_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any x_zeroing_speed = std::any(static_cast<int32_t>(1000));
    std::any x_zeroing_position = std::any(static_cast<int32_t>(X_ZEROING_POSITION * 100));

    std::any y_lead_length = std::any(static_cast<int32_t>(800));
    std::any y_operational_speed = std::any(static_cast<int32_t>(800));
    std::any y_length_type = std::any(static_cast<uint16_t>(0)); // 0->diameter 1->perimeter
    std::any y_reverse_axis = std::any(static_cast<uint8_t>(0));
    std::any y_operational_current = std::any(static_cast<int32_t>(1000));
    std::any y_idle_behavior = std::any(static_cast<uint16_t>(0));
    std::any y_sleep_current = std::any(static_cast<int32_t>(500));
    std::any y_auto_zreoing = std::any(static_cast<uint8_t>(0));
    std::any y_zeroing_reverse_dir = std::any(static_cast<uint8_t>(0));
    std::any y_zeroing_torch_thres = std::any(static_cast<int32_t>(128));
    std::any y_zeroing_current = std::any(static_cast<int32_t>(500));
    std::any y_zeroing_speed = std::any(static_cast<int32_t>(1000));
    std::any y_zeroing_position = std::any(static_cast<int32_t>(Y_ZEROING_POSITION * 100));
    std::any y_punch_depth = std::any(static_cast<int32_t>(Y_PUNCH_MOVEMENT_LENGTH * 100));

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
    QueueHandle_t evt_queue;
    TaskHandle_t evt_loop;
    friend void evtHandleLoop(void *param);
    QueueHandle_t setting_queue;
    TaskHandle_t setting_loop;
    friend void settingHandleLoop(void *param);

public:
    PuncherScheduler();
    ~PuncherScheduler();

    void begin();
    void beginNVS();

    /* Attach motors after begin() ! */
    inline void attachMotors(MotorController *X, MotorController *Y, MotorController *Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;

        if (has_begin)
        {
            initMotors();
        }
    }

    inline void attachPositionSensors(PositionSensor *X, PositionSensor *Y, PositionSensor *Z)
    {
        this->sensor_X = X;
        this->sensor_Y = Y;
        this->sensor_Z = Z;

        if (has_begin)
        {
            initSensors();
        }
    }

    /* Attach UI after begin() ! */
    inline void attachUI(PuncherUI *ui)
    {
        this->ui_list.push_back(ui);
        if (has_begin)
        {
            ui->onStatusCode(&status);
            ui->attachScheduler(this);
        }
    }

    /* update status to UI */
    inline void updateUIstatus()
    {
        for (PuncherUI *p_ui : this->ui_list)
        {
            p_ui->onStatusCode(&status);
        }
    }

    /* Attach PowerManager BEFORE begin() ! */
    inline void attachPower(PowerManager *pm)
    {
        this->pm = pm;
    }

    /* From PuncherSchedulerInterface */
    int start_workload();
    int pause_workload();
    int delete_workload();
    int data_transmit_mode(bool transmit_mode);
    int feed_paper_mode(bool feed_paper_mode);
    int add_hole(scheduler_hole_t &h);
    int add_hold_mcode(int x, int y, int z);
    int feed_paper(int gear, bool slow_mode);
    int util_move_X(int dir, bool use_zeroing_speed);
    int util_move_Y(int dir, bool use_zeroing_speed);
    int read_sg_result(int axis);     // 0 -> All axis; 0b1 -> X; 0b10 -> Y; 0b100 -> Z
    int start_auto_zeroing(int axis); // Same as above
    int start_encoder_calibrate(int axis);
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
    bool setXZeroingReverseDir(std::any val);
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
    bool setYZeroingReverseDir(std::any val);
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
    inline uint8_t getZencoderEnabled() { return std::any_cast<uint8_t>(z_encoder_enable); }
    inline uint16_t getZencoderType() { return std::any_cast<uint16_t>(z_encoder_type); }
};

#endif
