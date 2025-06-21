#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos_cpu_monitor.h"

#define NUM_OF_SPIN_TASKS 6
#define SPIN_ITER 500000
#define SPIN_TASK_PRIO 2

static char task_names[NUM_OF_SPIN_TASKS][configMAX_TASK_NAME_LEN];

static void spin_task(void *arg)
{
    while (1)
    {
        for (int i = 0; i < SPIN_ITER; i++)
        {
            __asm__ __volatile__("NOP");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    // Create sample spin tasks
    for (int i = 0; i < NUM_OF_SPIN_TASKS; i++)
    {
        snprintf(task_names[i], configMAX_TASK_NAME_LEN, "spin%d", i);
        xTaskCreatePinnedToCore(spin_task, task_names[i], 1024, NULL, SPIN_TASK_PRIO, NULL, tskNO_AFFINITY);
    }
    // Initialize the FreeRTOS CPU monitor (creates stats task)
    freertos_cpu_monitor_init();
}
