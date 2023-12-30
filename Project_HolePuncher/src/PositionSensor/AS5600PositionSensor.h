#ifndef _AS5600_POSITIONSENSOR_H_
#define _AS5600_POSITIONSENSOR_H_

#include <AS5600.h>
#include "PositionSensor.h"
#include "PuncherSemaphore.h"

class AS5600PositionSensor : public PositionSensor
{
private:
    int last_raw_angle;
    int64_t raw_relative_pos;

    AS5600 *sensor;
    SemaphoreHandle_t *port_mutex;
    SemaphoreHandle_t self_mutex;

    friend void as5600loop(void *param);
public:
    AS5600PositionSensor(TwoWire *i2c_port = &Wire, SemaphoreHandle_t *port_mutex = &I2C0Mutex);
    ~AS5600PositionSensor();

    void begin();
    position_res_t ping();
    /* in RAD */
    double getAngle();
    /* set current position to zero */
    void clearRelativePosition();
    /* degree since last setZero() */
    double getRelativePosition();
};


#endif  // _AS5600_POSITIONSENSOR_H_