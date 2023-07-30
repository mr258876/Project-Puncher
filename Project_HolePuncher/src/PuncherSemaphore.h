#ifndef _PUNCHERSEMAPHORE_H_
#define _PUNCHERSEMAPHORE_H_

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t I2CMutex;
extern SemaphoreHandle_t *LCDMutexptr;
extern SemaphoreHandle_t LVGLMutex;

#endif  // _PUNCHERSEMAPHORE_H_