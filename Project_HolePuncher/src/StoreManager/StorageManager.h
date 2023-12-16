#ifndef _STORAGEMANAGER_H_
#define _STORAGEMANAGER_H_

#include "esp_types.h"
#include "PuncherStorageTypes.h"

class PuncherStorageManager
{
private:
    const char *sector_name;
    uint32_t nvs_handle;

public:
    PuncherStorageManager(const char *sector_name);
    ~PuncherStorageManager();

    void begin();

    uint8_t get_uint8(const char *name);
    int8_t get_int8(const char *name);
    uint16_t get_uint16(const char *name);
    int16_t get_int16(const char *name);
    uint32_t get_uint32(const char *name);
    int32_t get_int32(const char *name);
    uint64_t get_uint64(const char *name);
    int64_t get_int64(const char *name);
    const char *get_string(const char *name);
    void get_blob(const char *name, void *ptr, size_t blob_size);
    float get_float(const char *name);
    double get_double(const char *name);
    bool get_bool(const char *name);

    void put_uint8(const char *name, uint8_t val);
    void put_int8(const char *name, int8_t val);
    void put_uint16(const char *name, uint16_t val);
    void put_int16(const char *name, int16_t val);
    void put_uint32(const char *name, uint32_t val);
    void put_int32(const char *name, int32_t val);
    void put_uint64(const char *name, uint64_t val);
    void put_int64(const char *name, int64_t val);
    void put_string(const char *name, const char *str);
    void put_blob(const char *name, void *data, size_t blob_size);
    void put_float(const char *name, float val);
    void put_double(const char *name, double val);
    void put_bool(const char *name, bool val);
};

#endif // _STORAGEMANAGER_H_