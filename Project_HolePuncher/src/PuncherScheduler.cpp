#include "PuncherScheduler.h"
#include "PuncherConf.h"

static const char *TAG = "PuncherScheduler";

void evtHandleLoop(void *param)
{
    PuncherScheduler *scheduler = (PuncherScheduler *)param;
    scheduler_evt_t evt;

    while (1)
    {
        xQueueReceive(scheduler->evt_queue, &evt, portMAX_DELAY);

        if (evt < 1)
        {
            ESP_LOGE(TAG, "Invalid event: %d", evt);
            continue;
        }

        if (evt <= EVT_ON_MOVE_FINISH_Z)
        {
            switch (evt)
            {
            case EVT_ON_MOVE_FINISH_X:
                scheduler->onFinishX();
                break;
            case EVT_ON_MOVE_FINISH_Y:
                scheduler->onFinishY();
                break;
            case EVT_ON_MOVE_FINISH_Z:
                scheduler->onFinishZ();
                break;
            default:
                break;
            }
            if (scheduler->status.basic_status.status_flags.is_running)
            {
                scheduler->handleMotorFinish();
            }
            continue;
        }

        if (evt <= EVT_ON_ZEROING_FINISH_Z)
        {
            switch (evt)
            {
            case EVT_ON_ZEROING_FINISH_X:
                scheduler->onFinishZeroingX();
                break;
            case EVT_ON_ZEROING_FINISH_Y:
                scheduler->onFinishZeroingY();
                break;
            case EVT_ON_ZEROING_FINISH_Z:
                scheduler->onFinishZeroingZ();
                break;
            default:
                break;
            }
            continue;
        }

        if (evt <= EVT_ON_WORKLOAD_DELETE)
        {
            switch (evt)
            {
            case EVT_ON_WORKLOAD_START:
                scheduler->start_workload_cb();
                break;
            case EVT_ON_WORKLOAD_PAUSE:
                scheduler->pause_workload_cb();
                break;
            case EVT_ON_WORKLOAD_DELETE:
                scheduler->delete_workload_cb();
                break;
            default:
                break;
            }
            continue;
        }

        switch (evt)
        {
        case EVT_ON_POWER_STATUS_CHANGE:
            scheduler->onPowerStatusChange();
            break;
        default:
            ESP_LOGE(TAG, "Invalid event: %d", evt);
            break;
        }
    }

    vTaskDelete(NULL);
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
    setting_mapping.emplace("x_zeroing_reverse_dir", puncher_setting_mapping_t(x_zeroing_reverse_dir, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                               { return this->setXZeroingReverseDir(val); }));
    setting_mapping.emplace("x_zeroing_torch_thres", puncher_setting_mapping_t(x_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setXZeroingTorchThres(val); }));
    setting_mapping.emplace("x_zeroing_current", puncher_setting_mapping_t(x_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                           { return this->setXZeroingCurrent(val); }));
    setting_mapping.emplace("x_zeroing_speed", puncher_setting_mapping_t(x_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setXZeroingSpeed(val); }));
    setting_mapping.emplace("x_zeroing_position", puncher_setting_mapping_t(x_zeroing_position, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                            { return this->setXZeroingPosition(val); }));

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
    setting_mapping.emplace("y_zeroing_reverse_dir", puncher_setting_mapping_t(y_zeroing_reverse_dir, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                               { return this->setYZeroingReverseDir(val); }));
    setting_mapping.emplace("y_zeroing_torch_thres", puncher_setting_mapping_t(y_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                               { return this->setYZeroingTorchThres(val); }));
    setting_mapping.emplace("y_zeroing_current", puncher_setting_mapping_t(y_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                           { return this->setYZeroingCurrent(val); }));
    setting_mapping.emplace("y_zeroing_speed", puncher_setting_mapping_t(y_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setYZeroingSpeed(val); }));
    setting_mapping.emplace("y_zeroing_position", puncher_setting_mapping_t(y_zeroing_position, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                            { return this->setYZeroingPosition(val); }));
    setting_mapping.emplace("y_punch_depth", puncher_setting_mapping_t(y_punch_depth, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                       { return this->setYPunchDepth(val); }));

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
    setting_mapping.emplace("z_cali_target_bar", puncher_setting_mapping_t(z_cali_target_bar, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                           { return this->setZCaliTargetBar(val); }));
    setting_mapping.emplace("z_cali_measure_bar", puncher_setting_mapping_t(z_cali_measure_bar, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                            { return this->setZCaliMeasureBar(val); }));
    setting_mapping.emplace("z_cali_residual", puncher_setting_mapping_t(z_cali_residual, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                         { return this->setZCaliResidual(val); }));
    setting_mapping.emplace("z_encoder_enable", puncher_setting_mapping_t(z_encoder_enable, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                                                          { return this->setZEncoderEnable(val); }));
    setting_mapping.emplace("z_encoder_type", puncher_setting_mapping_t(z_encoder_type, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                        { return this->setZEncoderType(val); }));

    setting_mapping.emplace("power_voltage", puncher_setting_mapping_t(power_voltage, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                       { return this->setPowerVoltage(val); }));

    setting_mapping.emplace("display_brightness", puncher_setting_mapping_t(display_brightness, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                            { return this->setDisplayBrightness(val); }));

    setting_mapping.emplace("display_language", puncher_setting_mapping_t(display_language, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                                                          { return this->setDisplayLanguage(val); }));

    setting_mapping.emplace("mcode_default_tick_rate", puncher_setting_mapping_t(mcode_default_tick_rate, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                                                                 { return this->setMcodeDefaultTickRate(val); }));

    ESP_LOGI(TAG, "Values len: %d", setting_mapping.size());
}

PuncherScheduler::~PuncherScheduler()
{
}

void PuncherScheduler::beginNVS()
{
    /*
        Start NVS
    */
    this->storage->begin();

    if (storage->get_uint8("initialized") == 0)
    {
        this->initSettings();
    }

    this->loadSettings();
}

void PuncherScheduler::begin()
{
    /*
        Init values
    */
    this->x_pos = std::any_cast<int32_t>(x_zeroing_position) / 100.0;
    this->y_pos = std::any_cast<int32_t>(y_zeroing_position) / 100.0;

    this->evt_queue = xQueueCreate(16, sizeof(scheduler_evt_t));
    // this->evt_queue = xQueueCreate(128, sizeof(puncher_event_t));
    xTaskCreate(evtHandleLoop, "SchedulerEvtLoop", 6144, this, 3, NULL);

    this->status.basic_status.status_data = 0;
    this->status.connectivity_status.status_data = 0;
    this->status.task_length = 0;
    this->status.finished_length = 0;
    this->status.ETA = 0;

    /*
        Init UI
    */
    initUI();

    /*
        Init Power
    */
    initPower();

    /*
        Init Motors
    */
    initMotors();

    /*
        Init Sensors
    */
    initSensors();

    has_begin = true;
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
    ESP_LOGD(TAG, "Save hash: %s", hexString);

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
    scheduler_evt_t evt = EVT_ON_WORKLOAD_START;
    xQueueSend(evt_queue, &evt, portMAX_DELAY);
    return 0;
}

int PuncherScheduler::pause_workload()
{
    scheduler_evt_t evt = EVT_ON_WORKLOAD_PAUSE;
    xQueueSend(evt_queue, &evt, portMAX_DELAY);
    return 0;
}

int PuncherScheduler::delete_workload()
{
    scheduler_evt_t evt = EVT_ON_WORKLOAD_DELETE;
    xQueueSend(evt_queue, &evt, portMAX_DELAY);
    return 0;
}

int PuncherScheduler::start_workload_cb()
{
    if (status.basic_status.status_data & (~PUNCHER_STATUS_HAS_MISSION))
    {
        ESP_LOGI(TAG, "Start workload fail! Status: %u", status.basic_status.status_data);
        return 1;
    }

    if (status.basic_status.status_flags.has_mission)
    {
        status.basic_status.status_flags.is_running = 1;

        ESP_LOGI(TAG, "Workload started!");
        updateUIstatus();

        z_pos = 0;
        z_target_pos = 0;
        if (sensor_Z_avaliable && std::any_cast<uint8_t>(z_encoder_enable))
        {
            sensor_Z->clearRelativePosition();
        }

        nextHole();

        return 0;
    }
    ESP_LOGI(TAG, "No mission avaliable!");
    return 1;
}

int PuncherScheduler::pause_workload_cb()
{
    // TODO

    return 0;
}

int PuncherScheduler::delete_workload_cb()
{
    if (status.basic_status.status_data & PUNCHER_STATUS_BUSY_MASK)
    {
        return this->status.basic_status.status_data;
    }

    holeList.clear();
    status.task_length = 0;
    status.finished_length = 0;
    status.basic_status.status_flags.has_mission = 0;

    return 0;
}

int PuncherScheduler::data_transmit_mode(bool transmit_mode)
{
    if (status.basic_status.status_data & (~(PUNCHER_STATUS_IS_TRANSMITTING_DATA | PUNCHER_STATUS_IS_FEEDING_PAPER)))
        return 1;

    status.basic_status.status_flags.is_transmitting_data = transmit_mode;

    if (transmit_mode)
    {
        holeList.clear();
    }
    else
    {
        status.task_length = holeList.size();
        status.finished_length = 0;
        if (status.task_length > 0)
            status.basic_status.status_flags.has_mission = 1;
    }

    updateUIstatus();
    return 0;
}

int PuncherScheduler::feed_paper_mode(bool feed_paper_mode)
{
    if (status.basic_status.status_data & (~(PUNCHER_STATUS_IS_FEEDING_PAPER | PUNCHER_STATUS_IS_TRANSMITTING_DATA)))
        return 1;

    status.basic_status.status_flags.is_feeding_paper = feed_paper_mode;
    if (feed_paper_mode)
    {
        Zawake();
        Z->sleep(true); // or motor is still powered after entering feeding mode
    }
    else
    {
        Z->sleep(false); // or motor might not powered after entering feeding mode
        Zsleep();
        if (sensor_Z)
            sensor_Z->clearRelativePosition();
    }

    return 0;
}

int PuncherScheduler::add_hole(scheduler_hole_t &h)
{
    /* Reject data if not in transmit mode */
    if (!status.basic_status.status_flags.is_transmitting_data)
        return 1;

    holeList.push_back(h);
    return 0;
}

int PuncherScheduler::add_hold_mcode(int x, int y, int z)
{
    if (x == 80)
        data_transmit_mode(1);

    /* Reject data if not in transmit mode */
    if (!status.basic_status.status_flags.is_transmitting_data)
        return 1;

    scheduler_hole_t hole;
    if (z < 1 || x == 80 || x == 90)
    {
        hole.x = 0;
    }
    else
    {
        hole.x = x;
    }

    hole.z = y * 1.0 * 8 / std::any_cast<int32_t>(mcode_default_tick_rate);

    int res = add_hole(hole);

    if (x == 90)
    {
        data_transmit_mode(0);
    }

    return res;
}

void PuncherScheduler::initMotors()
{
    ESP_LOGI("PuncherScheduler", "X driver status: %d", X->pingDriver());
    ESP_LOGI("PuncherScheduler", "Y driver status: %d", Y->pingDriver());
    ESP_LOGI("PuncherScheduler", "Z driver status: %d", Z->pingDriver());

    updateXspeed();
    updateYspeed();
    updateZspeed();

    updateXdriver();
    updateYdriver();
    updateZdriver();

    this->X->setMoveFinishCallBack([this]()
                                   { scheduler_evt_t evt = EVT_ON_MOVE_FINISH_X; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });
    this->Y->setMoveFinishCallBack([this]()
                                   { scheduler_evt_t evt = EVT_ON_MOVE_FINISH_Y; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });
    this->Z->setMoveFinishCallBack([this]()
                                   { scheduler_evt_t evt = EVT_ON_MOVE_FINISH_Z; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });

    Xsleep();
    Ysleep();
    Zsleep();
}

void PuncherScheduler::onPowerStatusChange()
{
    ESP_LOGI(TAG, "onPowerStatusChange");
    status.basic_status.status_flags.has_error = status_has_error();

    for (auto ui : ui_list)
    {
        ui->onStatusCode(&status);
    }
}

int PuncherScheduler::feed_paper(int gear)
{
    if (this->status.basic_status.status_data & (~PUNCHER_STATUS_IS_FEEDING_PAPER) & PUNCHER_STATUS_BUSY_MASK)
        return 1;

    if (gear)
    {
        int32_t z_speed = std::any_cast<int32_t>(this->z_operational_speed);
        z_speed = z_speed * abs(gear) / 3;

        this->Zawake();
        this->Z->sleep(false); // force power
        this->Z->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->z_lead_length),
            std::any_cast<uint16_t>(this->z_length_type),
            z_speed,
            MICROSTEPS_Z));
        this->Z->rotate_infinite(gear);
    }
    else
    {
        this->Z->stop();
        this->Z->sleep(true); // cut power
        updateZspeed();
    }

    return 0;
}

int PuncherScheduler::util_move_X(int dir, bool use_zeroing_conf)
{
    if (this->status.basic_status.status_data & (~PUNCHER_STATUS_IS_ZEROING) & PUNCHER_STATUS_BUSY_MASK)
        return 1;

    if (dir)
    {
        int32_t x_speed;
        if (use_zeroing_conf)
            x_speed = std::any_cast<int32_t>(this->x_zeroing_speed);
        else
            x_speed = std::any_cast<int32_t>(this->x_operational_speed);

        x_speed = x_speed * abs(dir);

        if (std::any_cast<uint8_t>(this->x_reverse_axis))
            dir = -dir;

        this->Xawake();

        if (use_zeroing_conf)
            this->X->setCurrent(std::any_cast<int32_t>(this->x_zeroing_current));
        else
            this->X->setCurrent(std::any_cast<int32_t>(this->x_operational_current));

        this->X->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->x_lead_length),
            std::any_cast<uint16_t>(this->x_length_type),
            x_speed,
            MICROSTEPS_X));

        this->status.basic_status.status_flags.is_zeroing_x = 1;
        this->X->rotate_infinite(dir);
    }
    else
    {
        this->X->stop();
        this->status.basic_status.status_flags.is_zeroing_x = 0;
        updateXspeed();
        updateXdriver();
        this->Xsleep();
    }

    return 0;
}

int PuncherScheduler::util_move_Y(int dir, bool use_zeroing_conf)
{
    if (this->status.basic_status.status_data & (~PUNCHER_STATUS_IS_ZEROING) & PUNCHER_STATUS_BUSY_MASK)
        return 1;

    if (dir)
    {
        int32_t y_speed;
        if (use_zeroing_conf)
            y_speed = std::any_cast<int32_t>(this->y_zeroing_speed);
        else
            y_speed = std::any_cast<int32_t>(this->y_operational_speed);

        y_speed = y_speed * abs(dir);

        if (std::any_cast<uint8_t>(this->y_reverse_axis))
            dir = -dir;

        this->Yawake();

        if (use_zeroing_conf)
            this->Y->setCurrent(std::any_cast<int32_t>(this->y_zeroing_current));
        else
            this->Y->setCurrent(std::any_cast<int32_t>(this->y_operational_current));

        this->Y->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->y_lead_length),
            std::any_cast<uint16_t>(this->y_length_type),
            y_speed,
            MICROSTEPS_Y));

        this->status.basic_status.status_flags.is_zeroing_y = 1;
        this->Y->rotate_infinite(dir);
    }
    else
    {
        this->Y->stop();
        this->status.basic_status.status_flags.is_zeroing_y = 0;
        updateYspeed();
        updateYdriver();
        this->Ysleep();
    }

    return 0;
}

/*
    @brief Read motor SG results
    @param axis: int, 0 -> All axis; 0b1 -> X; 0b10 -> Y; 0b100 -> Z
*/
int PuncherScheduler::read_sg_result(int axis)
{
    if (!axis)
        axis = ~axis;

    if (axis & 0b1)
    {
        int res = this->X->getLoad();
        puncher_event_setting_change_t e = {"x_sg_result", res};
        this->notifyValueChange(&e);
    }

    if (axis & 0b10)
    {
        int res = this->Y->getLoad();
        puncher_event_setting_change_t e = {"y_sg_result", res};
        this->notifyValueChange(&e);
    }

    if (axis & 0b100)
    {
        int res = this->Z->getLoad();
        puncher_event_setting_change_t e = {"z_sg_result", res};
        this->notifyValueChange(&e);
    }

    return 0;
}

int PuncherScheduler::start_auto_zeroing(int axis)
{
    if (this->status.basic_status.status_data & (~PUNCHER_STATUS_IS_ZEROING) & PUNCHER_STATUS_BUSY_MASK)
        return 1;

    if (!axis)
        axis = ~axis;

    if (axis & 0b1)
    {
        this->X->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->x_lead_length),
            std::any_cast<uint16_t>(this->x_length_type),
            std::any_cast<int32_t>(x_zeroing_speed),
            MICROSTEPS_X));
        this->X->setCurrent(std::any_cast<int32_t>(this->x_zeroing_current));
        this->Xawake();
        this->status.basic_status.status_flags.is_zeroing_x = 1;
        this->X->setZeroingFinishCallBack([this]()
                                          { scheduler_evt_t evt = EVT_ON_ZEROING_FINISH_X; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });
        this->X->startZeroing(std::any_cast<uint8_t>(x_zeroing_reverse_dir) ? -1 : 1, std::any_cast<int32_t>(x_zeroing_torch_thres));
    }

    if (axis & 0b10)
    {
        this->Y->setSpeed(calcMotorSpeedPulse(
            std::any_cast<int32_t>(this->y_lead_length),
            std::any_cast<uint16_t>(this->y_length_type),
            std::any_cast<int32_t>(y_zeroing_speed),
            MICROSTEPS_Y));
        this->Y->setCurrent(std::any_cast<int32_t>(this->y_zeroing_current));
        this->Yawake();
        this->status.basic_status.status_flags.is_zeroing_y = 1;
        this->Y->setZeroingFinishCallBack([this]()
                                          { scheduler_evt_t evt = EVT_ON_ZEROING_FINISH_Y; BaseType_t xHigherPriorityTaskWoken; xQueueSendFromISR(this->evt_queue, &evt, &xHigherPriorityTaskWoken); });
        this->Y->startZeroing(std::any_cast<uint8_t>(y_zeroing_reverse_dir) ? -1 : 1, std::any_cast<int32_t>(y_zeroing_torch_thres));
    }

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

    double _lastX = x_pos;

    for (size_t i = 0; i < holeList.size(); i++)
    {
        _eta += abs(holeList.at(i).z) / (std::any_cast<int32_t>(z_operational_speed) / 100.0 * (std::any_cast<uint16_t>(z_length_type) ? 1 : 3.14159265358979)); // Z轴
        if (holeList.at(i).x > 0)
        {
            _eta += abs((30 - holeList.at(i).x) * 2.0 - _lastX) / (std::any_cast<int32_t>(x_operational_speed) / 100.0 * (std::any_cast<uint16_t>(x_length_type) ? 1 : 3.14159265358979));           // X轴
            _eta += 2.0 * (std::any_cast<int32_t>(y_punch_depth) / 100.0) / (std::any_cast<int32_t>(y_operational_speed) / 100.0 * (std::any_cast<uint16_t>(y_length_type) ? 1 : 3.14159265358979)); // Y轴
            _lastX = (30 - holeList.at(i).x) * 2.0;
        }
    }

    return (long)_eta;
}

void PuncherScheduler::set_setting_value(puncher_event_setting_change_t *evt)
{
    puncher_setting_mapping_t item_mapping = setting_mapping.at(evt->item_name);
    if (item_mapping.call_back(evt->data))
    {
        saveValue(evt->item_name, item_mapping);

        notifyValueChange(evt);
    }
}

/* TODO: fix mem leak */
void PuncherScheduler::get_setting_values(void *p_ui)
{
    PuncherUI *ui = (PuncherUI *)p_ui;

    ESP_LOGD(TAG, "Values len: %d", setting_mapping.size());
    for (auto &pair : setting_mapping)
    {
        ESP_LOGD(TAG, "Pushing value %s", pair.first.c_str());

        puncher_event_setting_change_t evt = puncher_event_setting_change_t(pair.first.c_str(), pair.second.obj);

        ui->onSettingValueChange(&evt);
    }
    ESP_LOGD(TAG, "Setting values pushed to ui");
}

void PuncherScheduler::handleMotorFinish()
{
    double y_depth = std::any_cast<int32_t>(y_punch_depth) / 100.0;
    if ((x_finished && z_finished))
    {
        if (y_finished == 0)
        {
            // Move down Y
            Yawake();
            Y->move(calc_Y_steps(y_depth - y_pos));
            y_target_pos = y_depth;

            y_status = 1;
        }
        else
        {
            if (y_status == 1)
            {
                // Y movoed down, now move back
                Yawake();
                Y->move(calc_Y_steps(-y_depth));
                y_target_pos -= y_depth;

                y_status = 2;
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
    if (!status.basic_status.status_flags.is_running)
        return 1;

    if (holeList.size() < 1)
    {
        status.basic_status.status_flags.is_running = 0;
        status.basic_status.status_flags.has_mission = 0;
        status.task_length = 0;
        status.finished_length = 0;
        updateUIstatus();
        return 0;
    }

    scheduler_hole_t hole = holeList.front();

    x_finished = 0;
    y_finished = 0;
    z_finished = 0;

    y_status = 0;

    if (hole.x > 0)
    {
        ESP_LOGI(TAG, "Next X: %d", hole.x);
        int32_t x_steps = calc_X_steps((30 - hole.x) * 2.0 - x_pos);
        if (x_steps)
        {
            Xawake();
            X->move(x_steps);
            x_target_pos = (30 - hole.x) * 2.0;
            ESP_LOGI(TAG, "X current pos: %4f", x_pos);
            ESP_LOGI(TAG, "X target pos: %4f", x_target_pos);
        }
        else
        {
            x_finished = 1;
            x_target_pos = x_pos;
            scheduler_evt_t evt = EVT_ON_ZEROING_FINISH_Z;
            xQueueSend(this->evt_queue, &evt, portMAX_DELAY);
        }
    }
    else
    {
        x_finished = 1;
        x_target_pos = x_pos;
        y_finished = 1;
        y_target_pos = y_pos;
        y_status = 2;

        scheduler_evt_t evt = EVT_ON_ZEROING_FINISH_X;
        xQueueSend(this->evt_queue, &evt, portMAX_DELAY);
        evt = EVT_ON_ZEROING_FINISH_Y;
        xQueueSend(this->evt_queue, &evt, portMAX_DELAY);
    }

    if (hole.z > 0)
    {
        z_target_pos += hole.z;
        Zawake();
        Z->move(calc_Z_steps(hole.z));
    }
    else
    {
        z_finished = 1;
        z_target_pos = z_pos;

        scheduler_evt_t evt = EVT_ON_ZEROING_FINISH_Z;
        xQueueSend(this->evt_queue, &evt, portMAX_DELAY);
    }

    holeList.erase(holeList.begin());

    status.finished_length += 1;
    status.ETA = get_ETA();
    updateUIstatus();

    return 0;
}

void PuncherScheduler::onFinishX()
{
    Xsleep();
    x_finished = 1;
    x_pos = x_target_pos;
}

void PuncherScheduler::onFinishY()
{
    Ysleep();
    y_finished = 1;
    y_pos = y_target_pos;
}

void PuncherScheduler::onFinishZ()
{
    if (sensor_Z_avaliable && std::any_cast<uint8_t>(z_encoder_enable))
    {
        double pos = sensor_Z->getRelativePosition();
        double pos_target = z_target_pos / (std::any_cast<int32_t>(z_lead_length) * 1.0 / 100) / (std::any_cast<uint16_t>(z_length_type) ? 1 : 3.14159265358979) * 2 * 3.14159265358979;
        pos_target = pos_target * (std::any_cast<int32_t>(z_cali_target_bar) * 8.0 / (std::any_cast<int32_t>(z_cali_measure_bar) * 8.0 + std::any_cast<int32_t>(z_cali_residual) / 1000.0));
        long diff_steps = (pos_target - pos) / 2 / 3.14159265358979 * MOTOR_STEPS * MICROSTEPS_Z;

        if (diff_steps > 2 || diff_steps < -2)
        {
            // move the extra steps
            Z->move(diff_steps);
            return;
        }
    }

    Zsleep();
    z_finished = 1;
    z_pos = z_target_pos;
}

void PuncherScheduler::Xsleep()
{
    switch (std::any_cast<uint16_t>(x_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->X->setCurrent(std::any_cast<int32_t>(this->x_sleep_current));
        break;
    case 2:
        this->X->sleep(true);
        break;
    default:
        break;
    }
}
// Leave idle mode
void PuncherScheduler::Xawake()
{
    switch (std::any_cast<uint16_t>(x_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->X->setCurrent(std::any_cast<int32_t>(this->x_operational_current));
        break;
    case 2:
        this->X->sleep(false);
        break;
    default:
        break;
    }
}
// Enter idle mode
void PuncherScheduler::Ysleep()
{
    switch (std::any_cast<uint16_t>(y_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->Y->setCurrent(std::any_cast<int32_t>(this->y_sleep_current));
        break;
    case 2:
        this->Y->sleep(true);
        break;
    default:
        break;
    }
}
// Leave idle mode
void PuncherScheduler::Yawake()
{
    switch (std::any_cast<uint16_t>(y_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->Y->setCurrent(std::any_cast<int32_t>(this->y_operational_current));
        break;
    case 2:
        this->Y->sleep(false);
        break;
    default:
        break;
    }
}
// Enter idle mode
void PuncherScheduler::Zsleep()
{
    switch (std::any_cast<uint16_t>(z_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->Z->setCurrent(std::any_cast<int32_t>(this->z_sleep_current));
        break;
    case 2:
        this->Z->sleep(true);
        break;
    default:
        break;
    }
}
// Leave idle mode
void PuncherScheduler::Zawake()
{
    switch (std::any_cast<uint16_t>(z_idle_behavior))
    {
    case 0:
        break;
    case 1:
        this->Z->setCurrent(std::any_cast<int32_t>(this->z_operational_current));
        break;
    case 2:
        this->Z->sleep(false);
        break;
    default:
        break;
    }
}