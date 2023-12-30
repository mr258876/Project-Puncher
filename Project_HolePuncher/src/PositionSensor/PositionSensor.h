#ifndef _POSITIONSENSOR_H_
#define _POSITIONSENSOR_H_

enum position_res_t
{
    POSITION_RES_NOT_IMPLEMENTED = -1,
    POSITION_RES_SUCCESS = 0,
    POSITION_RES_FAIL,
};

class PositionSensor
{
public:
    virtual void begin();
    virtual position_res_t ping();
    /* in RAD */
    virtual double getAngle();
    /* set current position to zero */
    virtual void clearRelativePosition();
    /* degree since last setZero() */
    virtual double getRelativePosition();
};


#endif