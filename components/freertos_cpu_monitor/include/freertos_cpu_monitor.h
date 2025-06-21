#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "freertos/FreeRTOS.h"

    // Initializes the CPU monitor (creates tasks, semaphores, etc.)
    void freertos_cpu_monitor_init(void);

    // Optionally, expose the stats print function if needed externally
    esp_err_t freertos_cpu_monitor_print_stats(TickType_t xTicksToWait);

#ifdef __cplusplus
}
#endif
