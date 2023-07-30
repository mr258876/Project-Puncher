#include "PuncherSemaphore.h"

SemaphoreHandle_t I2CMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t LVGLMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t *LCDMutexptr = &I2CMutex;