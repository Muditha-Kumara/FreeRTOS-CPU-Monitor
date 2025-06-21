# FreeRTOS CPU Monitor Component

A reusable ESP-IDF component for monitoring CPU and task statistics in FreeRTOS-based ESP32 projects.

## Usage

Include the header and call the initialization function in your project:

```c
#include "freertos_cpu_monitor.h"

void app_main() {
    freertos_cpu_monitor_init();
}
```
