#include "PuncherScheduler.h"

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

    std::vector<puncher_setting_mapping_t> setting_mapping = {
        puncher_setting_mapping_t("x_lead_length", x_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_lead_length = val; }),
        puncher_setting_mapping_t("x_operational_speed", x_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_operational_speed = val; }),
        puncher_setting_mapping_t("x_reverse_axis", x_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                  { this->x_reverse_axis = val; }),
        puncher_setting_mapping_t("x_operational_current", x_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_operational_current = val; }),
        puncher_setting_mapping_t("x_idle_behavior", x_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                  { this->x_idle_behavior = val; }),
        puncher_setting_mapping_t("x_sleep_current", x_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_sleep_current = val; }),
        puncher_setting_mapping_t("x_auto_zreoing", x_auto_zreoing, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                  { this->x_auto_zreoing = val; }),
        puncher_setting_mapping_t("x_zeroing_torch_thres", x_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_zeroing_torch_thres = val; }),
        puncher_setting_mapping_t("x_zeroing_current", x_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_zeroing_current = val; }),
        puncher_setting_mapping_t("x_zeroing_speed", x_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->x_zeroing_speed = val; }),

        puncher_setting_mapping_t("y_lead_length", y_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_lead_length = val; }),
        puncher_setting_mapping_t("y_operational_speed", y_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_operational_speed = val; }),
        puncher_setting_mapping_t("y_reverse_axis", y_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                  { this->y_reverse_axis = val; }),
        puncher_setting_mapping_t("y_operational_current", y_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_operational_current = val; }),
        puncher_setting_mapping_t("y_idle_behavior", y_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                  { this->y_idle_behavior = val; }),
        puncher_setting_mapping_t("y_sleep_current", y_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_sleep_current = val; }),
        puncher_setting_mapping_t("y_auto_zreoing", y_auto_zreoing, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                  { this->y_auto_zreoing = val; }),
        puncher_setting_mapping_t("y_zeroing_torch_thres", y_zeroing_torch_thres, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_zeroing_torch_thres = val; }),
        puncher_setting_mapping_t("y_zeroing_current", y_zeroing_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_zeroing_current = val; }),
        puncher_setting_mapping_t("y_zeroing_speed", y_zeroing_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->y_zeroing_speed = val; }),

        puncher_setting_mapping_t("z_lead_length", z_lead_length, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->z_lead_length = val; }),
        puncher_setting_mapping_t("z_operational_speed", z_operational_speed, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->z_operational_speed = val; }),
        puncher_setting_mapping_t("z_reverse_axis", z_reverse_axis, PUNCHER_STORAGE_TYPE_UINT8, [this](std::any val)
                                  { this->z_reverse_axis = val; }),
        puncher_setting_mapping_t("z_operational_current", z_operational_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->z_operational_current = val; }),
        puncher_setting_mapping_t("z_idle_behavior", z_idle_behavior, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                  { this->z_idle_behavior = val; }),
        puncher_setting_mapping_t("z_sleep_current", z_sleep_current, PUNCHER_STORAGE_TYPE_INT32, [this](std::any val)
                                  { this->z_sleep_current = val; }),

        puncher_setting_mapping_t("power_voltage", power_voltage, PUNCHER_STORAGE_TYPE_UINT16, [this](std::any val)
                                  { this->power_voltage = val; }),
    };
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
}

void PuncherScheduler::loadSettings()
{
    std::hash<std::string> hashStr;

    for (auto item : setting_mapping)
    {
        size_t hashVal = hashStr(item.obj_name);

        const char *hexString = uint32ToHex(hashVal).c_str();

        switch (item.type)
        {
        case PUNCHER_STORAGE_TYPE_UINT8:
            item.call_back(storage->get_uint8(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT8:
            item.call_back(storage->get_int8(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT16:
            item.call_back(storage->get_uint16(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT16:
            item.call_back(storage->get_int16(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT32:
            item.call_back(storage->get_uint32(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT32:
            item.call_back(storage->get_int32(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_UINT64:
            item.call_back(storage->get_uint64(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_INT64:
            item.call_back(storage->get_int64(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_STRING:
            item.call_back(storage->get_string(hexString));
            break;
        case PUNCHER_STORAGE_FLOAT:
            item.call_back(storage->get_float(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_DOUBLE:
            item.call_back(storage->get_double(hexString));
            break;
        case PUNCHER_STORAGE_TYPE_BOOL:
            item.call_back(storage->get_bool(hexString));
            break;
        default:
            break;
        }
    }
}

void PuncherScheduler::initSettings()
{
    // Save default values
    for (auto item : setting_mapping)
    {
        this->saveValue(item);
    }

    storage->put_uint8("initialized", 1);
}

void PuncherScheduler::saveValue(puncher_setting_mapping_t item)
{
    std::hash<std::string> hashStr;
    size_t hashVal = hashStr(item.obj_name);

    const char *hexString = uint32ToHex(hashVal).c_str();

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
    holeList.push_back(h);
    return 0;
}

int PuncherScheduler::feed_paper(int gear)
{
    if (puncher_is_busy(this->status))
        return this->status.basic_status.status_data;

    // TODO
    if (gear)
    {
        // this->Z.sleep(false);
        this->Z->rotate_infinite(gear);
        this->status.basic_status.status_flags.is_feeding_paper = 1;
    }
    else
    {
        this->Z->stop();
        // this->Z.sleep(true);
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
    if (evt->item_name == NULL)
    {
        /* code */
        return;
    }
}