# Project Puncher - 控制器程序 / Controller Program

## 硬件需求 / Hardware requirements
| 物品 / Item | 数量 / Amount |
|---|---|
| ESP32-S3-N16R8 | 1 |
| TMC2209 Stepper Driver | 3 |
| P028B123-10-IPS-CTP / P028X101-10-CTP 2.8' LCD with Touchscreen | 1 |
| PCF8574T | 1 |
| CH224K | 1 |

## 支持库 / Libraries Used
### 3rd party libraries:
- [lvgl/lvgl@^8.4.0](https://github.com/lvgl/lvgl)
- [teemuatlut/TMCStepper@^0.7.3](https://github.com/teemuatlut/TMCStepper)
- [robtillaart/PCF8574@^0.3.9](https://github.com/robtillaart/PCF8574)
### Modified & Distributed under MIT license:
- [owennewo/as5600_i2c_arduino](https://github.com/owennewo/as5600_i2c_arduino)
- [strange-v/FT6X36](https://github.com/strange-v/FT6X36)

## Compilation Note

### lv_i18n commands
Run `compile` in this folder to generate i18n files:
```bash
lv_i18n compile -t 'i18n/*.yml' -o 'src/ui/i18n'
```

### FATFS page code warning
In `.platformio/packages/framework-espidf/components/fatfs/src/ff.c`
Line 541
```patch
-541 static const BYTE Ct855[] = TBL_CT855;
+541 // static const BYTE Ct855[] = TBL_CT855;
```

### Arduino TinyUSB Header BUG
In `.platformio/packages/framework-arduinoespressif32/cores/esp32/USBCDC.cpp`

Line 18-19
```patch
-18 #include "USB.h"
-19 #if CONFIG_TINYUSB_CDC_ENABLED
+18 #include "USB.h"
+19 #if CONFIG_TINYUSB_CDC_ENABLED && CONFIG_TINYUSB_ENABLED
```

### stringop-truncation warning
In `.platformio/packages/framework-arduinoespressif32/cores/esp32/MacAddress.cpp`

Line 74-76
```patch
+74 #pragma GCC diagnostic push
+   #pragma GCC diagnostic ignored "-Wstringop-truncation"
 75 strncpy(cs, buf, sizeof(cs));  //strtok modifies the buffer: copy to working buffer.
+76 #pragma GCC diagnostic pop

+93 #pragma GCC diagnostic push
+   #pragma GCC diagnostic ignored "-Wstringop-truncation"
 94 strncpy(cs, buf, sizeof(cs));  //strtok modifies the buffer: copy to working buffer.
+95 #pragma GCC diagnostic pop
```

### printf format warning
In `.pio/libdeps/esp32s3/NimBLE-Arduino/src/nimble/nimble/host/src/ble_gap.c`
 
Line 318
```patch
-318 BLE_HS_LOG(INFO, "duration=%dms", duration_ms);
+318 BLE_HS_LOG(INFO, "duration=%ldms", duration_ms);
```