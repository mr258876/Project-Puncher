#include "PuncherScheduler.h"

void evtHandleLoop(void *param)
{
    PuncherScheduler *scheduler = (PuncherScheduler *)param;
    while (1)
    {
        uint32_t evt = xEventGroupWaitBits(scheduler->motor_evt_group, 0xFF, pdTRUE, pdFALSE, portMAX_DELAY);

        if (evt & OnFinishX)
            scheduler->x_finished = 1;
        if (evt & OnFinishY)
            scheduler->y_finished = 1;
        if (evt & OnFinishZ)
            scheduler->z_finished = 1;

        if (scheduler->status.basic_status.status_flags.is_running)
        {
            scheduler->handleMotorFinish();
        }
    }
}

std::string uint32ToHex(uint32_t val)
{
    const char hexChars[] = "0123456789ABCDEF";
    std::string result;
    do
    {
        result = hexChars[val % 16] + result;
        val /= 16;
    } while (val != 0);

    while (result.length() < 8)
    {
        result = '0' + result;
    }

    return result;
}

PuncherScheduler::PuncherScheduler()
{
    this->holeListHandle = xSemaphoreCreateMutex();
    this->storage = new PuncherStorageManager("PuncherNG");

    // setting_mapping
    setting_mapping.emplace("x_lead_length", puncher_setting_mapping_t(x_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                       { return this->setXLeadLength(val); }));
    setting_mapping.emplace("x_operational_speed", puncher_setting_mapping_t(x_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                             { return this->setXOperationalSpeed(val); }));
    setting_mapping.emplace("x_length_type", puncher_setting_mapping_t(x_length_type, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                       { return this->setXLengthType(val); }));
    setting_mapping.emplace("x_reverse_axis", puncher_setting_mapping_t(x_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                        { return this->setXReverseAxis(val); }));
    setting_mapping.emplace("x_operational_current", puncher_setting_mapping_t(x_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setXOperationalCurrent(val); }));
    setting_mapping.emplace("x_idle_behavior", puncher_setting_mapping_t(x_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                         { return this->setXIdleBehavior(val); }));
    setting_mapping.emplace("x_sleep_current", puncher_setting_mapping_t(x_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setXSleepCurrent(val); }));
    setting_mapping.emplace("x_auto_zreoing", puncher_setting_mapping_t(x_auto_zreoing, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                        { return this->setXAutoZreoing(val); }));
    setting_mapping.emplace("x_zeroing_torch_thres", puncher_setting_mapping_t(x_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setXZeroingTorchThres(val); }));
    setting_mapping.emplace("x_zeroing_current", puncher_setting_mapping_t(x_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                           { return this->setXZeroingCurrent(val); }));
    setting_mapping.emplace("x_zeroing_speed", puncher_setting_mapping_t(x_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setXZeroingSpeed(val); }));

    setting_mapping.emplace("y_lead_length", puncher_setting_mapping_t(y_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                       { return this->setYLeadLength(val); }));
    setting_mapping.emplace("y_operational_speed", puncher_setting_mapping_t(y_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                             { return this->setYOperationalSpeed(val); }));
    setting_mapping.emplace("y_length_type", puncher_setting_mapping_t(y_length_type, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                       { return this->setYLengthType(val); }));
    setting_mapping.emplace("y_reverse_axis", puncher_setting_mapping_t(y_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                        { return this->setYReverseAxis(val); }));
    setting_mapping.emplace("y_operational_current", puncher_setting_mapping_t(y_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setYOperationalCurrent(val); }));
    setting_mapping.emplace("y_idle_behavior", puncher_setting_mapping_t(y_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                         { return this->setYIdleBehavior(val); }));
    setting_mapping.emplace("y_sleep_current", puncher_setting_mapping_t(y_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setYSleepCurrent(val); }));
    setting_mapping.emplace("y_auto_zreoing", puncher_setting_mapping_t(y_auto_zreoing, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                        { return this->setYAutoZreoing(val); }));
    setting_mapping.emplace("y_zeroing_torch_thres", puncher_setting_mapping_t(y_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setYZeroingTorchThres(val); }));
    setting_mapping.emplace("y_zeroing_current", puncher_setting_mapping_t(y_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                           { return this->setYZeroingCurrent(val); }));
    setting_mapping.emplace("y_zeroing_speed", puncher_setting_mapping_t(y_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setYZeroingSpeed(val); }));

    setting_mapping.emplace("z_lead_length", puncher_setting_mapping_t(z_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                       { return this->setZLeadLength(val); }));
    setting_mapping.emplace("z_operational_speed", puncher_setting_mapping_t(z_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                             { return this->setZOperationalSpeed(val); }));
    setting_mapping.emplace("z_length_type", puncher_setting_mapping_t(z_length_type, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                       { return this->setZLengthType(val); }));
    setting_mapping.emplace("z_reverse_axis", puncher_setting_mapping_t(z_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                        { return this->setZReverseAxis(val); }));
    setting_mapping.emplace("z_operational_current", puncher_setting_mapping_t(z_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setZOperationalCurrent(val); }));
    setting_mapping.emplace("z_idle_behavior", puncher_setting_mapping_t(z_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                         { return this->setZIdleBehavior(val); }));
    setting_mapping.emplace("z_sleep_current", puncher_setting_mapping_t(z_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setZSleepCurrent(val); }));

    setting_mapping.emplace("power_voltage", puncher_setting_mapping_t(power_voltage, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                       { return this->setPowerVoltage(val); }));

    setting_mapping.emplace("display_brightness", puncher_setting_mapping_t(display_brightness, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                            { return this->setDisplayBrightness(val); }));

    setting_mapping.emplace("display_language", puncher_setting_mapping_t(display_language, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                          { return this->setDisplayLanguage(val); }));

    ESP_LOGI("PuncherScheduler", "Values len: %d", setting_mapping.size());
}

PuncherScheduler::~PuncherScheduler()
{
}

void PuncherScheduler::begin()
{
    this->storage->begin();

    if (storage->get_uint8("initialized") == 0)
    {
        this->initSettings();
    }

    this->loadSettings();

    this->motor_evt_group = xEventGroupCreate();
    // this->evt_queue = xQueueCreate(128, sizeof(puncher_event_t));
    xTaskCreate(evtHandleLoop, "SchedulerEvtLoop", 8192, this, 3, NULL);

    this->status.basic_status.status_data = 0;
    this->status.connectivity_status.status_data = 0;
}

void PuncherScheduler::loadSettings()
{
    ESP_LOGI("PuncherSeheduler", "Loading values from NVS");
    std::hash<std::string> hashStr;

    for (auto &pair : setting_mapping)
    {
        size_t hashVal = hashStr(pair.first);

        const char *hexString = uint32ToHex(hashVal).c_str();

        switch (pair.second.type)
        {
        case PUNCHER_STORAGE_TYPE_UINT8:
            pair.second.call_back(storage->get_uint8(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT8:
            pair.second.call_back(storage->get_int8(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT16:
            pair.second.call_back(storage->get_uint16(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT16:
            pair.second.call_back(storage->get_int16(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT32:
            pair.second.call_back(storage->get_uint32(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT32:
            pair.second.call_back(storage->get_int32(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT64:
            pair.second.call_back(storage->get_uint64(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT64:
            pair.second.call_back(storage->get_int64(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_STRING:
            pair.second.call_back(storage->get_string(hexString));
            break;
        case PUNCHER_STORAGE_FLOAT:
            pair.second.call_back(storage->get_float(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_DOUBLE:
            pair.second.call_back(storage->get_double(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_BOOL:
            pair.second.call_back(storage->get_bool(hexString));
            break;
        default:
            break;
        }
    }

    ESP_LOGI("PuncherSeheduler", "NVS values loaded");
}

void PuncherScheduler::initSettings()
{
    ESP_LOGI("PuncherSeheduler", "NVS not initialized, saving default values");

    // Save default values
    for (auto &pair : setting_mapping)
    {
        this->saveValue(pair.first, pair.second);
    }

    storage->put_uint8("initialized", 1);
}

void PuncherScheduler::saveValue(std::string name, puncher_setting_mapping_t item)
{
    std::hash<std::string> hashStr;
    size_t hashVal = hashStr(name);

    const char *hexString = uint32ToHex(hashVal).c_str();
    ESP_LOGI("PuncherScheduler", "Save hash: %s", hexString);

    switch (item.type)
    {
    case PUNCHER_STORAGE_TYPE_UINT8:
        storage->put_uint8(hexString, std::any_cast<uint8_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_INT8:
        storage->put_int8(hexString, std::any_cast<int8_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_UINT16:
        storage->put_uint16(hexString, std::any_cast<uint16_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_INT16:
        storage->put_int16(hexString, std::any_cast<int16_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_UINT32:
        storage->put_uint32(hexString, std::any_cast<uint32_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_INT32:
        storage->put_int32(hexString, std::any_cast<int32_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_UINT64:
        storage->put_uint64(hexString, std::any_cast<uint64_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_INT64:
        storage->put_int64(hexString, std::any_cast<int64_t>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_STRING:
        storage->put_string(hexString, std::any_cast<std::string>(item.obj).c_str());
        break;
    case PUNCHER_STORAGE_FLOAT:
        storage->put_float(hexString, std::any_cast<float>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_DOUBLE:
        storage->put_double(hexString, std::any_cast<double>(item.obj));
        break;
    case PUNCHER_STORAGE_TYPE_BOOL:
        storage->put_bool(hexString, std::any_cast<uint8_t>(item.obj));
        break;
    default:
        break;
    }
}

int PuncherScheduler::start_workload()
{
    if (puncher_is_busy(status))
        return status.basic_status.status_data;

    if (status.basic_status.status_flags.has_mission)
    {
        reverse(holeList.begin(), holeList.end()); // Reverse the vector so it will be O(1) each time poping a hole

        // TODO

        status.basic_status.status_flags.is_running = 1;
        return 0;
    }
    return 1;
}

int PuncherScheduler::pause_workload()
{
    // TODO

    return 0;
}

int PuncherScheduler::delete_workload()
{
    if (puncher_is_busy(this->status))
    {
        return this->status.basic_status.status_data;
    }

    this->holeList.clear();
    return 0;
}

int PuncherScheduler::add_hole(scheduler_hole_t h)
{
    /* Reject data if not in transmit mode */
    if (!status.basic_status.status_flags.is_transmitting_data)
        return 1;

    holeList.push_back(h);
    return 0;
}

int PuncherScheduler::feed_paper(int gear)
{
    if (puncher_is_busy(this->status))
        return this->status.basic_status.status_data;

    if (gear)
    {
        int32_t z_speed = std::any_cast<int32_t>(this->z_operational_speed);
        z_speed = z_speed * abs(gear) / 2;

        this->Z->sleep(false);
        this->Z->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->z_lead_length),
            std::any_cast<uint16_t>(this->z_length_type),
            z_speed,
            64));
        this->Z->rotate_infinite(gear);
        this->status.basic_status.status_flags.is_feeding_paper = 1;
    }
    else
    {
        this->Z->stop();
        this->Z->sleep(true);
        updateZspeed();
        this->status.basic_status.status_flags.is_feeding_paper = 0;
    }

    return 0;
}

int PuncherScheduler::ui_get_menu()
{
    // TODO

    return 0;
}

unsigned int PuncherScheduler::set_status(unsigned int status_code)
{
    // TODO

    return status.basic_status.status_data;
}

unsigned int PuncherScheduler::get_status()
{
    return status.basic_status.status_data;
}

time_t PuncherScheduler::get_ETA()
{
    double _eta = 0;

    int _lastX = 0;
    double _lastZ = 0;

    // for (size_t i = 0; i < holeList.size(); i++)
    // {
    //     _eta += abs(holeList.at(i).x - _lastX) * 2.0 / X->getSpeed(); // X轴
    //     _eta += abs(holeList.at(i).z - _lastZ) / Z->getSpeed();       // Y轴
    //     _eta += 11 / Y->getSpeed();                                   // Z轴

    //     _lastX = holeList.at(i).x;
    //     _lastZ = holeList.at(i).z;
    // }

    return (long)_eta;
}

void PuncherScheduler::set_setting_value(puncher_event_setting_change_t *evt)
{
    puncher_setting_mapping_t item_mapping = setting_mapping.at(evt->item_name);
    if (item_mapping.call_back(evt->data))
    {
        saveValue(evt->item_name, item_mapping);

        /* Notify all UI after value change */
        for (auto &ui : this->ui_list)
        {
            ui->onSettingValueChange(evt);
        }
    }
}

/* TODO: fix mem leak */
void PuncherScheduler::get_setting_values(void *p_ui)
{
    PuncherUI *ui = (PuncherUI *)p_ui;

    ESP_LOGD("PuncherScheduler", "Values len: %d", setting_mapping.size());
    for (auto &pair : setting_mapping)
    {
        ESP_LOGD("PuncherScheduler", "Pushing value %s", pair.first.c_str());

        puncher_event_setting_change_t evt = puncher_event_setting_change_t(pair.first.c_str(), pair.second.obj);

        ui->onSettingValueChange(&evt);
    }
    ESP_LOGD("PuncherScheduler", "Setting values pushed to ui");
}

void PuncherScheduler::handleMotorFinish()
{
    if ((x_finished && z_finished))
    {
        if (y_finished == 0)
        {
            // Move down Y
            /* TODO */
            y_status = 1;
            y_finished = 0;
        }
        else
        {
            if (y_status == 1)
            {
                // Y movoed down, now move back
                /* TODO */
                y_status = 2;
                y_finished = 0;
            }
            else
            {
                // Y movements finished, load next hole
                y_status = 0;

                nextHole();
            }
        }
    }
}

int PuncherScheduler::nextHole()
{
    if (!status.basic_status.status_flags.is_running) return 1;
    
    if (holeList.size() < 1)
    {
        status.basic_status.status_flags.is_running = 0;
        return 0;
    }
    
    scheduler_hole_t hole = holeList.front();

    x_finished = 0;
    y_finished = 0;
    z_finished = 0;

    if (hole.x > 0)
    {
        // X->move();
    }
    double z_steps = (hole.z - z_pos) / (std::any_cast<int32_t>(z_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(z_length_type) ? 1 : 3.14159265358979) * 200 * 64;
    z_steps += 0.5;
    Z->move((int32_t)z_steps);

    holeList.erase(holeList.begin());

    return 0;
}