#include "PuncherScheduler.h"

void PuncherScheduler::setXLeadLength(std::any val)
{
    this->x_lead_length = val;
}
void PuncherScheduler::setXOperationalSpeed(std::any val)
{
    this->x_operational_speed = val;
}
void PuncherScheduler::setXReverseAxis(std::any val)
{
    this->x_reverse_axis = val;
}
void PuncherScheduler::setXOperationalCurrent(std::any val)
{
    this->x_operational_current = val;
}
void PuncherScheduler::setXIdleBehavior(std::any val)
{
    this->x_idle_behavior = val;
}
void PuncherScheduler::setXSleepCurrent(std::any val)
{
    this->x_sleep_current = val;
}
void PuncherScheduler::setXAutoZreoing(std::any val)
{
    this->x_auto_zreoing = val;
}
void PuncherScheduler::setXZeroingTorchThres(std::any val)
{
    this->x_zeroing_torch_thres = val;
}
void PuncherScheduler::setXZeroingCurrent(std::any val)
{
    this->x_zeroing_current = val;
}
void PuncherScheduler::setXZeroingSpeed(std::any val)
{
    this->x_zeroing_speed = val;
}

void PuncherScheduler::setYLeadLength(std::any val)
{
    this->y_lead_length = val;
}
void PuncherScheduler::setYOperationalSpeed(std::any val)
{
    this->y_operational_speed = val;
}
void PuncherScheduler::setYReverseAxis(std::any val)
{
    this->y_reverse_axis = val;
}
void PuncherScheduler::setYOperationalCurrent(std::any val)
{
    this->y_operational_current = val;
}
void PuncherScheduler::setYIdleBehavior(std::any val)
{
    this->y_idle_behavior = val;
}
void PuncherScheduler::setYSleepCurrent(std::any val)
{
    this->y_sleep_current = val;
}
void PuncherScheduler::setYAutoZreoing(std::any val)
{
    this->y_auto_zreoing = val;
}
void PuncherScheduler::setYZeroingTorchThres(std::any val)
{
    this->y_zeroing_torch_thres = val;
}
void PuncherScheduler::setYZeroingCurrent(std::any val)
{
    this->y_zeroing_current = val;
}
void PuncherScheduler::setYZeroingSpeed(std::any val)
{
    this->y_zeroing_speed = val;
}

void PuncherScheduler::setZLeadLength(std::any val)
{
    this->z_lead_length = val;
}
void PuncherScheduler::setZOperationalSpeed(std::any val)
{
    this->z_operational_speed = val;
}
void PuncherScheduler::setZReverseAxis(std::any val)
{
    this->z_reverse_axis = val;
}
void PuncherScheduler::setZOperationalCurrent(std::any val)
{
    this->z_operational_current = val;
}
void PuncherScheduler::setZIdleBehavior(std::any val)
{
    this->z_idle_behavior = val;
}
void PuncherScheduler::setZSleepCurrent(std::any val)
{
    this->z_sleep_current = val;
}

void PuncherScheduler::setPowerVoltage(std::any option)
{
    this->power_voltage = option;
    pm_acquire_voltage(std::any_cast<uint16_t>(option));
}