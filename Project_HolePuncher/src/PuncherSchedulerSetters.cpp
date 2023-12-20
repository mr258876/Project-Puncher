#include "PuncherScheduler.h"

bool PuncherScheduler::setXLeadLength(std::any val)
{
    if (std::any_cast<int32_t>(this->x_lead_length) == std::any_cast<int32_t>(val))
        return false;

    this->x_lead_length = val;
    updateXspeed();
    return true;
}
bool PuncherScheduler::setXOperationalSpeed(std::any val)
{
    if (std::any_cast<int32_t>(this->x_operational_speed) == std::any_cast<int32_t>(val))
        return false;

    this->x_operational_speed = val;
    updateXspeed();
    return true;
}
bool PuncherScheduler::setXLengthType(std::any val)
{
    if (std::any_cast<uint16_t>(this->x_length_type) == std::any_cast<uint16_t>(val))
        return false;

    this->x_length_type = val;
    updateXspeed();
    return true;
}
bool PuncherScheduler::setXReverseAxis(std::any val)
{
    if (std::any_cast<uint8_t>(this->x_reverse_axis) == std::any_cast<uint8_t>(val))
        return false;

    this->x_reverse_axis = val;
    return true;
}
bool PuncherScheduler::setXOperationalCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->x_operational_current) == std::any_cast<int32_t>(val))
        return false;

    this->x_operational_current = val;
    updateXdriver();
    return true;
}
bool PuncherScheduler::setXIdleBehavior(std::any val)
{
    if (std::any_cast<uint16_t>(this->x_idle_behavior) == std::any_cast<uint16_t>(val))
        return false;

    this->x_idle_behavior = val;
    return true;
}
bool PuncherScheduler::setXSleepCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->x_sleep_current) == std::any_cast<int32_t>(val))
        return false;

    this->x_sleep_current = val;
    return true;
}
bool PuncherScheduler::setXAutoZreoing(std::any val)
{
    if (std::any_cast<uint8_t>(this->x_auto_zreoing) == std::any_cast<uint8_t>(val))
        return false;

    this->x_auto_zreoing = val;
    return true;
}
bool PuncherScheduler::setXZeroingTorchThres(std::any val)
{
    if (std::any_cast<int32_t>(this->x_zeroing_torch_thres) == std::any_cast<int32_t>(val))
        return false;

    this->x_zeroing_torch_thres = val;
    return true;
}
bool PuncherScheduler::setXZeroingCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->x_zeroing_current) == std::any_cast<int32_t>(val))
        return false;

    this->x_zeroing_current = val;
    return true;
}
bool PuncherScheduler::setXZeroingSpeed(std::any val)
{
    if (std::any_cast<int32_t>(this->x_zeroing_speed) == std::any_cast<int32_t>(val))
        return false;

    this->x_zeroing_speed = val;
    return true;
}

bool PuncherScheduler::setYLeadLength(std::any val)
{
    if (std::any_cast<int32_t>(this->y_lead_length) == std::any_cast<int32_t>(val))
        return false;

    this->y_lead_length = val;
    updateYspeed();
    return true;
}
bool PuncherScheduler::setYOperationalSpeed(std::any val)
{
    if (std::any_cast<int32_t>(this->y_operational_speed) == std::any_cast<int32_t>(val))
        return false;

    this->y_operational_speed = val;
    updateYspeed();
    return true;
}
bool PuncherScheduler::setYLengthType(std::any val)
{
    if (std::any_cast<uint16_t>(this->y_length_type) == std::any_cast<uint16_t>(val))
        return false;

    this->y_length_type = val;
    updateYspeed();
    return true;
}
bool PuncherScheduler::setYReverseAxis(std::any val)
{
    if (std::any_cast<uint8_t>(this->y_reverse_axis) == std::any_cast<uint8_t>(val))
        return false;

    this->y_reverse_axis = val;
    return true;
}
bool PuncherScheduler::setYOperationalCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->y_operational_current) == std::any_cast<int32_t>(val))
        return false;

    this->y_operational_current = val;
    updateYdriver();
    return true;
}
bool PuncherScheduler::setYIdleBehavior(std::any val)
{
    if (std::any_cast<uint16_t>(this->y_idle_behavior) == std::any_cast<uint16_t>(val))
        return false;

    this->y_idle_behavior = val;
    return true;
}
bool PuncherScheduler::setYSleepCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->y_sleep_current) == std::any_cast<int32_t>(val))
        return false;

    this->y_sleep_current = val;
    return true;
}
bool PuncherScheduler::setYAutoZreoing(std::any val)
{
    if (std::any_cast<uint8_t>(this->y_auto_zreoing) == std::any_cast<uint8_t>(val))
        return false;

    this->y_auto_zreoing = val;
    return true;
}
bool PuncherScheduler::setYZeroingTorchThres(std::any val)
{
    if (std::any_cast<int32_t>(this->y_zeroing_torch_thres) == std::any_cast<int32_t>(val))
        return false;

    this->y_zeroing_torch_thres = val;
    return true;
}
bool PuncherScheduler::setYZeroingCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->y_zeroing_current) == std::any_cast<int32_t>(val))
        return false;

    this->y_zeroing_current = val;
    return true;
}
bool PuncherScheduler::setYZeroingSpeed(std::any val)
{
    if (std::any_cast<int32_t>(this->y_zeroing_speed) == std::any_cast<int32_t>(val))
        return false;

    this->y_zeroing_speed = val;
    return true;
}

bool PuncherScheduler::setZLeadLength(std::any val)
{
    if (std::any_cast<int32_t>(this->z_lead_length) == std::any_cast<int32_t>(val))
        return false;

    this->z_lead_length = val;
    updateZspeed();
    return true;
}
bool PuncherScheduler::setZOperationalSpeed(std::any val)
{
    if (std::any_cast<int32_t>(this->z_operational_speed) == std::any_cast<int32_t>(val))
        return false;

    this->z_operational_speed = val;
    updateZspeed();
    return true;
}
bool PuncherScheduler::setZLengthType(std::any val)
{
    if (std::any_cast<uint16_t>(this->z_length_type) == std::any_cast<uint16_t>(val))
        return false;

    this->z_length_type = val;
    updateZspeed();
    return true;
}
bool PuncherScheduler::setZReverseAxis(std::any val)
{
    if (std::any_cast<uint8_t>(this->z_reverse_axis) == std::any_cast<uint8_t>(val))
        return false;

    this->z_reverse_axis = val;
    return true;
}
bool PuncherScheduler::setZOperationalCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->z_operational_current) == std::any_cast<int32_t>(val))
        return false;

    this->z_operational_current = val;
    updateZdriver();
    return true;
}
bool PuncherScheduler::setZIdleBehavior(std::any val)
{
    if (std::any_cast<uint16_t>(this->z_idle_behavior) == std::any_cast<uint16_t>(val))
        return false;

    this->z_idle_behavior = val;
    return true;
}
bool PuncherScheduler::setZSleepCurrent(std::any val)
{
    if (std::any_cast<int32_t>(this->z_sleep_current) == std::any_cast<int32_t>(val))
        return false;

    this->z_sleep_current = val;
    return true;
}

bool PuncherScheduler::setPowerVoltage(std::any option)
{
    if (std::any_cast<uint16_t>(this->power_voltage) == std::any_cast<uint16_t>(option))
        return false;

    this->power_voltage = option;
    pm_acquire_voltage(std::any_cast<uint16_t>(option));
    return true;
}

bool PuncherScheduler::setDisplayBrightness(std::any val)
{
    if (std::any_cast<int32_t>(this->display_brightness) == std::any_cast<int32_t>(val))
        return false;

    this->display_brightness = val;
    return true;
}

bool PuncherScheduler::setDisplayLanguage(std::any option)
{
    if (std::any_cast<uint16_t>(this->display_language) == std::any_cast<uint16_t>(option))
        return false;

    this->display_language = option;
    return true;
}