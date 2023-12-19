#include "PuncherScheduler.h"

uint32_t PuncherScheduler::calcMotorSpeedPulse(int32_t length10UM, uint16_t length_type, int32_t speed10UMpS)
{
    return length10UM / speed10UMpS * (length_type ? 1 : 3.14159265358979);
}

uint32_t PuncherScheduler::calcMotorSpeedPulse(std::any length10UM, std::any length_type, std::any speed10UMpS)
{
    return calcMotorSpeedPulse(std::any_cast<int32_t>(length10UM), std::any_cast<uint16_t>(length_type), std::any_cast<int32_t>(speed10UMpS));
}