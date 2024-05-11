#ifndef _AS5600_POSITIONSENSOR_H_
#define _AS5600_POSITIONSENSOR_H_

#include <AS5600.h>
#include "PositionSensor.h"
#include "PuncherSemaphore.h"

class AS5600PositionSensor : public PositionSensor
{
private:
    int16_t last_raw_angle;
    int64_t raw_relative_pos;

    AS5600 *sensor;
    SemaphoreHandle_t *port_mutex;
    SemaphoreHandle_t self_mutex;

    bool use_cali_profile = false;
    uint16_t cali_profile[512];

    std::function<void()> cal_fn_cb;

    /* in RAD */
    double _getAngle();

    friend void as5600loop(void *param);
    friend void tsk_axis_cali(void *param);

public:
    AS5600PositionSensor(TwoWire *i2c_port = &Wire, SemaphoreHandle_t *port_mutex = &I2C0Mutex);
    ~AS5600PositionSensor();

    void begin();
    position_res_t ping();
    /* in RAD */
    inline double getAngle() { return last_raw_angle * 1.0 / 4096; };
    /* uint16_t */
    inline int16_t rawReading() { return last_raw_angle; };
    /* set current position to zero */
    void clearRelativePosition();
    /* degree since last setZero() */
    double getRelativePosition();
    /* load calibration profile */
    position_res_t loadCalibrationProfile(void *profile);
    // void *getCalibrationProfile() {return NULL;};
    size_t getCalibrationProfileSize();

    position_res_t startCalibration(MotorController *axis, uint32_t microsteps_per_revolution);
    position_res_t setCalibrationFinishCallBack(std::function<void()> cb);
};

#endif // _AS5600_POSITIONSENSOR_H_