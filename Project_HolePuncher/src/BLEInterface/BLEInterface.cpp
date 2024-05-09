#include "BLEInterface.h"

static const char *TAG = "BLEInterface";

// Callbacks for server events
class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer)
    {
        ESP_LOGI(TAG, "Client connected");
    }

    void onDisconnect(NimBLEServer *pServer)
    {
        ESP_LOGI(TAG, "Client disconnected");
    }

    void onAuthenticationComplete(ble_gap_conn_desc *desc)
    {
        ESP_LOGI(TAG, "Pairing complete, peer address %s, type %d", NimBLEAddress(desc->peer_id_addr).toString().c_str(), desc->peer_id_addr.type);
        ESP_LOGI(TAG, "Bond count %d", NimBLEDevice::getNumBonds());
    }
};

BLEInterface::BLEInterface(/* args */)
{
}

BLEInterface::~BLEInterface()
{
}

void BLEInterface::begin()
{
    start();
}

void BLEInterface::start()
{
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    NimBLEDevice::init("Project Puncher NG");

    // Enable IRK distribution from both ends
    NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);

    // Enable pairing & RPA
    NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_PUBLIC, false);
    NimBLEDevice::setSecurityAuth(true, true, true);
    // NimBLEDevice::setSecurityAuth(true, false, true);   // disable mitm for Bleak

    pBLEServer = NimBLEDevice::createServer();

    pBLEAdvertising = NimBLEDevice::getAdvertising();
    mcode_setup();
    pBLEAdvertising->start();

    pBLEServer->setCallbacks(new ServerCallbacks());
}

void BLEInterface::stop()
{
    NimBLEDevice::stopAdvertising();
    NimBLEDevice::deinit(true);
    this->pBLEAdvertising = nullptr;
    this->pBLEServer = nullptr;
    esp_bt_controller_disable();
}

void BLEInterface::onStatusCode(puncher_status_t *data)
{
}
void BLEInterface::onSettingValueChange(puncher_event_setting_change_t *data)
{
}

void BLEInterface::attachScheduler(PuncherSchedulerInterface *p_scheduler)
{
    this->scheduler = p_scheduler;
}

BLEInterface *bleInterface = new BLEInterface();