#include "StorageManager.h"
#include <nvs_flash.h>
#include <esp_log.h>

static const char *log_tag = "StorageManager";

static void read_err_handler(esp_err_t err, const char *name)
{
    switch (err)
    {
    case ESP_OK:
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        ESP_LOGW(log_tag, "Value %s is not initialized yet!", name);
        break;
    default:
        ESP_LOGE(log_tag, "Error reading value %s!", name);
        break;
    }
}

static void write_err_handler(esp_err_t err, const char *name)
{
    switch (err)
    {
    case ESP_OK:
        break;
    default:
        ESP_LOGE(log_tag, "Error writing value %s!", name);
        break;
    }
}

PuncherStorageManager::PuncherStorageManager(const char *sector_name)
{
    this->sector_name = sector_name;
}

PuncherStorageManager::~PuncherStorageManager()
{
}

void PuncherStorageManager::begin()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(nvs_open(sector_name, NVS_READWRITE, &nvs_handle));
}

uint8_t PuncherStorageManager::get_uint8(const char *name)
{
    uint8_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u8(this->nvs_handle, name, &val);
    read_err_handler(err, name);

    return val;
}

int8_t PuncherStorageManager::get_int8(const char *name)
{
    int8_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i8(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

uint16_t PuncherStorageManager::get_uint16(const char *name)
{
    uint16_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u16(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

int16_t PuncherStorageManager::get_int16(const char *name)
{
    int16_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i16(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

uint32_t PuncherStorageManager::get_uint32(const char *name)
{
    uint32_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u32(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

int32_t PuncherStorageManager::get_int32(const char *name)
{
    int32_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i32(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

uint64_t PuncherStorageManager::get_uint64(const char *name)
{
    uint64_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u64(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

int64_t PuncherStorageManager::get_int64(const char *name)
{
    int64_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i64(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    return val;
}

const char *PuncherStorageManager::get_string(const char *name)
{
    size_t str_len = 0;
    nvs_get_str(this->nvs_handle, name, NULL, &str_len);
    char *str = (char *)malloc(str_len);
    nvs_get_str(this->nvs_handle, "server_name", str, &str_len);
    return str;
}

void PuncherStorageManager::get_blob(const char *name, void *ptr, size_t blob_size)
{
    esp_err_t err = nvs_get_blob(this->nvs_handle, name, ptr, &blob_size);
    read_err_handler(err, name);
}

float PuncherStorageManager::get_float(const char *name)
{
    uint64_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u64(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    float val_float = *(float *)&val;
    return val_float;
}

double PuncherStorageManager::get_double(const char *name)
{
    uint64_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u64(this->nvs_handle, name, &val);
    read_err_handler(err, name);
    double val_double = *(double *)&val;
    return val_double;
}

bool PuncherStorageManager::get_bool(const char *name)
{
    uint8_t val = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_u8(this->nvs_handle, name, &val);
    read_err_handler(err, name);

    return val;
}

void PuncherStorageManager::put_uint8(const char *name, uint8_t val)
{
    esp_err_t err = nvs_set_u8(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_int8(const char *name, int8_t val)
{
    esp_err_t err = nvs_set_i8(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_uint16(const char *name, uint16_t val)
{
    esp_err_t err = nvs_set_u16(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_int16(const char *name, int16_t val)
{
    esp_err_t err = nvs_set_i16(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_uint32(const char *name, uint32_t val)
{
    esp_err_t err = nvs_set_u32(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_int32(const char *name, int32_t val)
{
    esp_err_t err = nvs_set_i32(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_uint64(const char *name, uint64_t val)
{
    esp_err_t err = nvs_set_u64(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_int64(const char *name, int64_t val)
{
    esp_err_t err = nvs_set_i64(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_string(const char *name, const char *str)
{
    esp_err_t err = nvs_set_str(this->nvs_handle, name, str);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_blob(const char *name, void *data, size_t blob_size)
{
    esp_err_t err = nvs_set_blob(this->nvs_handle, name, data, blob_size);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_float(const char *name, float val)
{
    uint64_t val_u64 = *(uint64_t *)&val;
    esp_err_t err = nvs_set_u64(this->nvs_handle, name, val_u64);
    read_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_double(const char *name, double val)
{
    uint64_t val_u64 = *(uint64_t *)&val;
    esp_err_t err = nvs_set_u64(this->nvs_handle, name, val_u64);
    read_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}

void PuncherStorageManager::put_bool(const char *name, bool val)
{
    esp_err_t err = nvs_set_u8(this->nvs_handle, name, val);
    write_err_handler(err, name);
    err = nvs_commit(this->nvs_handle);
    write_err_handler(err, name);
}