#include "SerialInterface.h"
#include <stdint.h>
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "esp_log.h"
#include "tusb_console.h"

static void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event);
static void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event);
static void tinyusb_cdc_line_coding_changed_callback(int itf, cdcacm_event_t *event);

static const char *TAG = "SerialInterface";
static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

SerialInterface::SerialInterface(/* args */)
{
}

SerialInterface::~SerialInterface()
{
}

void SerialInterface::begin()
{
    ESP_LOGI(TAG, "USB initialization");
    tinyusb_config_t tusb_cfg = {}; // the configuration using default values
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback,
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = &tinyusb_cdc_line_state_changed_callback,
        .callback_line_coding_changed = &tinyusb_cdc_line_coding_changed_callback,
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));

    ESP_LOGI(TAG, "USB initialization DONE");
}

void SerialInterface::enableLog()
{
    ESP_ERROR_CHECK(esp_tusb_init_console(TINYUSB_CDC_ACM_0));
}

void SerialInterface::onStatusCode(puncher_event_code_t *data) {}
void SerialInterface::onSettingValueChange(puncher_event_setting_change_t *data) {}
void SerialInterface::attachScheduler(PuncherSchedulerInterface *p_scheduler)
{
    this->scheduler = p_scheduler;
    scheduler->get_setting_values(this);
}

SerialInterface *usbInterface = new SerialInterface();

static void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    /* initialization */
    size_t rx_size = 0;

    /* read */
    esp_err_t ret = tinyusb_cdcacm_read((tinyusb_cdcacm_itf_t)itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK)
    {
        buf[rx_size] = '\0';
        ESP_LOGD(TAG, "Got data (%d bytes): %s", rx_size, buf);
    }
    else
    {
        ESP_LOGE(TAG, "Read error");
    }

    /* write back */
    tinyusb_cdcacm_write_queue((tinyusb_cdcacm_itf_t)itf, buf, rx_size);
    tinyusb_cdcacm_write_flush((tinyusb_cdcacm_itf_t)itf, 0);
}

static void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
{
    int dtr = event->line_state_changed_data.dtr;
    int rst = event->line_state_changed_data.rts;
    ESP_LOGI(TAG, "Line state changed! dtr:%d, rst:%d", dtr, rst);
}

static void tinyusb_cdc_line_coding_changed_callback(int itf, cdcacm_event_t *event)
{
    int bit_rate = event->line_coding_changed_data.p_line_coding->bit_rate;
    int data_bits = event->line_coding_changed_data.p_line_coding->data_bits;
    int parity = event->line_coding_changed_data.p_line_coding->parity;
    int stop_bits = event->line_coding_changed_data.p_line_coding->stop_bits;
    ESP_LOGI(TAG, "Line coding changed! bit_rate:%d, data_bits:%d, parity:%d, stop_bits:%d", bit_rate, data_bits, parity, stop_bits);
}