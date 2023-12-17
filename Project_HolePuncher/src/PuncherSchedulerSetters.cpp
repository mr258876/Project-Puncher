#include "PuncherScheduler.h"

void PuncherScheduler::setPowerVoltage(std::any option)
{
    this->power_voltage = option;
    pm_acquire_voltage(std::any_cast<uint16_t>(option));
}