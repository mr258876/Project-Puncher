#include "SerialInterface.h"
#include <Arduino.h>
#include <USB.h>

SerialInterface::SerialInterface(/* args */)
{
}

SerialInterface::~SerialInterface()
{
}

void SerialInterface::begin()
{
    USBSerial.begin();
    USB.begin();
}

void SerialInterface::onStatusCode(puncher_event_code_t *data) {}
void SerialInterface::onSettingValueChange(puncher_event_setting_change_t *data) {}
void SerialInterface::attachScheduler(PuncherSchedulerInterface *p_scheduler) {}