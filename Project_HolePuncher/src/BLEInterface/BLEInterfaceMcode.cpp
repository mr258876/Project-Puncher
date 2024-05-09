#include "BLEInterface.h"

static const char *TAG = "BLEInterface";

static const NimBLEUUID BLE_MCODE_SERVICE_UUID(0x00003200, 0x0000, 0x1000, 0xD44130F244F2E354); // 0xD44130F244F2E354 is 16-bit MD5 of "Project Puncher NG"
static const NimBLEUUID BLE_MCODE_CHAR_UUID(0x00003201, 0x0000, 0x1000, 0xD44130F244F2E354);

// Callback class for handling API characteristic read and write events
class McodeInCharacteristicCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *pCharacteristic)
    {
        /* read */
        NimBLEAttValue value = pCharacteristic->getValue();
        if (value.length() > 0)
        {
            ESP_LOGD(TAG, "Got data (%d bytes): %s", value.length(), value.c_str());

            int x, y, z;
            int scan_res = sscanf(value.c_str(), "M%d Y%d P%d", &x, &y, &z);
            ESP_LOGD(TAG, "Mcode sscanf Result: %d", scan_res);
            int add_res = 0;
            if (scan_res == 3)
            {
                add_res = bleInterface->getScheduler()->add_hold_mcode(x, y, z);
                if (add_res)
                {
                    pCharacteristic->notify("OK");
                }
                else
                {
                    pCharacteristic->notify("S1");
                }
            }
        }
        else
        {
            ESP_LOGE(TAG, "Read error");
        }
    }
};

// Callback class for handling API characteristic read and write events
class McodeIndicateCharacteristicCallbacks : public NimBLECharacteristicCallbacks
{
    void onStatus(NimBLECharacteristic *pCharacteristic, Status s, int code)
    {
        ESP_LOGI(TAG, "Notification/Indication return code: %d, %s\n", code, NimBLEUtils::returnCodeToString(code));
    }
};

void BLEInterface::mcode_setup()
{
    // Setup current time service
    NimBLEService *pMcodeService = pBLEServer->createService(BLE_MCODE_SERVICE_UUID);
    NimBLECharacteristic *pMcodeCharacteristic = pMcodeService->createCharacteristic(
        BLE_MCODE_CHAR_UUID,
        // NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_ENC | NIMBLE_PROPERTY::NOTIFY // Only allow paired devices to write
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY // Only allow paired devices to write
    );
    pMcodeCharacteristic->setCallbacks(new McodeInCharacteristicCallbacks());
    pMcodeService->start();

    // Add service to advertising
    pBLEAdvertising->addServiceUUID(pMcodeService->getUUID());
}