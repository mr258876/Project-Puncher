#include "PuncherSemaphore.h"

SemaphoreHandle_t I2C0Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t I2C1Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t SPI2Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t SPI3Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t LVGLMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t *LCDMutexptr = &SPI2Mutex;
SemaphoreHandle_t UART0Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t UART1Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t UART2Mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t *DUARTMutex = &UART0Mutex;