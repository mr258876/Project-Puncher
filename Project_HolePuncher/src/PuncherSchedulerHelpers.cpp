#include "PuncherScheduler.h"
#include "PuncherConf.h"

uint32_t PuncherScheduler::calcMotorSpeedPulse(int32_t length10UM, uint16_t length_type, int32_t speed10UMpS, int32_t microSteps)
{
    double res = speed10UMpS * 1.0;
    res = res / length10UM * (length_type ? 1 : 3.14159265358979) * MOTOR_STEPS * microSteps;
    res += 0.5;
    return (int32_t)res;
}

uint32_t PuncherScheduler::calcMotorSpeedPulse(std::any length10UM, std::any length_type, std::any speed10UMpS, std::any microSteps)
{
    return calcMotorSpeedPulse(std::any_cast<int32_t>(length10UM), std::any_cast<uint16_t>(length_type), std::any_cast<int32_t>(speed10UMpS), std::any_cast<int32_t>(microSteps));
}