#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos_cpu_monitor.h"

void app_main(void)
{
    printf("ESP-IDF project structure initialized.\n");
    freertos_cpu_monitor_init();
}
