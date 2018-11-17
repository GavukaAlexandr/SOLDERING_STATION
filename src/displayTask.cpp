#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "./../lib/max7219.h"

extern int encButtonState;
extern int ctrlTemp1;
extern int ctrlTemp2;
extern int ctrlFan;

extern int temperature1;
extern int temperature2;

extern SemaphoreHandle_t xEncButtonMutex;
extern SemaphoreHandle_t xCtrlTemp1Mutex;
extern SemaphoreHandle_t xCtrlTemp2Mutex;
extern SemaphoreHandle_t xCtrlFanMutex;
extern SemaphoreHandle_t xTemperature1Mutex;
extern SemaphoreHandle_t xTemperature2Mutex;

void TaskMainDisplay(void *pvParameters);

void displayTaskInit()
{
    max7219Init();
    xTaskCreate(
        TaskMainDisplay, (const portCHAR *)"TaskMainDisplay" // A name just for humans
        ,
        128 // ? This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

void displayState(
    SemaphoreHandle_t *xCtrlDataMutex,
    int *data,
    int displayNumber)
{
    xSemaphoreGive(xEncButtonMutex);
    xSemaphoreTake(*xCtrlDataMutex, (TickType_t)10);
    print(*data, displayNumber);
    xSemaphoreGive(*xCtrlDataMutex);
}

void TaskMainDisplay(void *pvParameters) // This is a task.
{
    (void)pvParameters;

    for (;;) // A Task shall never return or exit.
    {
        xSemaphoreTake(xEncButtonMutex, (TickType_t)10);

        if (encButtonState == 0)
        {
            xSemaphoreGive(xEncButtonMutex);
            xSemaphoreTake(xTemperature1Mutex, (TickType_t)10);
            xSemaphoreTake(xTemperature2Mutex, (TickType_t)10);
            xSemaphoreTake(xCtrlFanMutex, (TickType_t)10);

            print(temperature1, 1);
            print(temperature2, 2);
            print(ctrlFan, 3);

            xSemaphoreGive(xTemperature1Mutex);
            xSemaphoreGive(xTemperature2Mutex);
            xSemaphoreGive(xCtrlFanMutex);
        }
        else if (encButtonState == 1)
        {
            displayState(&xCtrlTemp1Mutex, &ctrlTemp1, 1);
        }
        else if (encButtonState == 2)
        {
            displayState(&xCtrlTemp2Mutex, &ctrlTemp2, 2);
        }
        else if (encButtonState == 3)
        {
            displayState(&xCtrlFanMutex, &ctrlFan, 3);
        }

        xSemaphoreGive(xEncButtonMutex);
    }
}
