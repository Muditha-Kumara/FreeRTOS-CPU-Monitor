#include "freertos_cpu_monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#define ARRAY_SIZE_OFFSET 5
#define STATS_TICKS pdMS_TO_TICKS(1000)
#define STATS_TASK_PRIO 3

static TaskHandle_t stats_task_handle = NULL;

static esp_err_t print_real_time_stats(TickType_t xTicksToWait)
{
    TaskStatus_t *start_array = NULL, *end_array = NULL;
    UBaseType_t start_array_size, end_array_size;
    configRUN_TIME_COUNTER_TYPE start_run_time, end_run_time;
    esp_err_t ret;

    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array = malloc(sizeof(TaskStatus_t) * start_array_size);
    if (start_array == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    start_array_size = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
    if (start_array_size == 0)
    {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    vTaskDelay(xTicksToWait);

    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = malloc(sizeof(TaskStatus_t) * end_array_size);
    if (end_array == NULL)
    {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    end_array_size = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);
    if (end_array_size == 0)
    {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    uint32_t total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0)
    {
        ret = ESP_ERR_INVALID_STATE;
        goto exit;
    }

    printf("| Task | Run Time | Percentage\n");
    for (int i = 0; i < start_array_size; i++)
    {
        int k = -1;
        for (int j = 0; j < end_array_size; j++)
        {
            if (start_array[i].xHandle == end_array[j].xHandle)
            {
                k = j;
                start_array[i].xHandle = NULL;
                end_array[j].xHandle = NULL;
                break;
            }
        }
        if (k >= 0)
        {
            uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time * CONFIG_FREERTOS_NUMBER_OF_CORES);
            printf("| %s | %" PRIu32 " | %" PRIu32 "%%\n", start_array[i].pcTaskName, task_elapsed_time, percentage_time);
        }
    }
    for (int i = 0; i < start_array_size; i++)
    {
        if (start_array[i].xHandle != NULL)
        {
            printf("| %s | Deleted\n", start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < end_array_size; i++)
    {
        if (end_array[i].xHandle != NULL)
        {
            printf("| %s | Created\n", end_array[i].pcTaskName);
        }
    }
    ret = ESP_OK;

exit:
    free(start_array);
    free(end_array);
    return ret;
}

static void stats_task(void *arg)
{
    while (1)
    {
        printf("\n\nGetting real time stats over %" PRIu32 " ticks\n", STATS_TICKS);
        if (print_real_time_stats(STATS_TICKS) == ESP_OK)
        {
            printf("Real time stats obtained\n");
        }
        else
        {
            printf("Error getting real time stats\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void freertos_cpu_monitor_init(void)
{
    if (stats_task_handle == NULL)
    {
        xTaskCreatePinnedToCore(stats_task, "stats", 4096, NULL, STATS_TASK_PRIO, &stats_task_handle, tskNO_AFFINITY);
    }
}

esp_err_t freertos_cpu_monitor_print_stats(TickType_t xTicksToWait)
{
    return print_real_time_stats(xTicksToWait);
}
