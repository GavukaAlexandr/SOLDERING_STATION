#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "./../lib/max6675lib.h"

extern int temperature1;
extern int temperature2;

extern SemaphoreHandle_t xTemperature1Mutex;
extern SemaphoreHandle_t xTemperature2Mutex;

void TaskGetTemperature(void *pvParameters);

void getTemperatureTaskInit()
{
    xTemperature1Mutex = xSemaphoreCreateMutex();
    xTemperature2Mutex = xSemaphoreCreateMutex();

    max6675Init();
    
    xTaskCreate(
        TaskGetTemperature, (const portCHAR *)"TaskGetTemperature" // A name just for humans
        ,
        65 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

void TaskGetTemperature(void *pvParameters) // This is a task.
{
    (void)pvParameters;

    while (1)
    {
        // xSemaphoreTake(xTemperature1Mutex, (TickType_t)10);
        // temperature1 = (int)(getByCelsius1());
        // xSemaphoreGive(xTemperature1Mutex);

        xSemaphoreTake(xTemperature2Mutex, (TickType_t)10);
        temperature2 = (int)(getByCelsius2());
        xSemaphoreGive(xTemperature2Mutex);

        vTaskDelay(50 / portTICK_PERIOD_MS); // wait for one second
    }
}
