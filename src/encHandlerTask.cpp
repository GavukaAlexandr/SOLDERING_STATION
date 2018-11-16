#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Encoder.h>

long oldPosition = 0;

extern int encButtonState;
extern int ctrlTemp1;
extern int ctrlTemp2;
extern int ctrlFan;

extern SemaphoreHandle_t xEncButtonMutex;
extern SemaphoreHandle_t xCtrlTemp2Mutex;
extern SemaphoreHandle_t xCtrlTemp1Mutex;
extern SemaphoreHandle_t xCtrlFanMutex;;

void TaskEncHandler(void *pvParameters);

int encCtrl(int limit);

Encoder myEnc(3, 7);

void encHandlerTaskInit()
{
    xCtrlTemp1Mutex = xSemaphoreCreateMutex();
    xCtrlTemp2Mutex = xSemaphoreCreateMutex();
    xCtrlFanMutex = xSemaphoreCreateMutex();

    xTaskCreate(
        TaskEncHandler, (const portCHAR *)"TaskEncHandler" // A name just for humans
        ,
        128 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

int encCtrl(int limit)
{
    long newPosition = myEnc.read();
    if (newPosition != oldPosition)
    {
        if (newPosition < 0)
            myEnc.write(0);
        if (newPosition > limit)
            myEnc.write(limit);
        oldPosition = newPosition;
    }
    return oldPosition;
}

void stateHandler(
    SemaphoreHandle_t *xCtrlParamMutex,
    int *ctrlTempParam,
    int thisState)
{
    xSemaphoreGive(xEncButtonMutex);
    xSemaphoreTake(*xCtrlParamMutex, (TickType_t)10);
    oldPosition = *ctrlTempParam;
    myEnc.write(*ctrlTempParam);
    xSemaphoreGive(*xCtrlParamMutex);

    while (encButtonState == thisState)
    {
        xSemaphoreTake(*xCtrlParamMutex, (TickType_t)10);
        *ctrlTempParam = encCtrl(500);
        xSemaphoreGive(*xCtrlParamMutex);
    }
}

void TaskEncHandler(void *pvParameters) // This is a task.
{
    (void)pvParameters;

    while (1)
    {
        xSemaphoreTake(xEncButtonMutex, (TickType_t)10);

        if (encButtonState == 0)
        {
        }
        else if (encButtonState == 1)
        {
            stateHandler(&xCtrlTemp1Mutex, &ctrlTemp1, 1);
        }
        else if (encButtonState == 2)
        {
            stateHandler(&xCtrlTemp2Mutex, &ctrlTemp2, 2);
        }
        else if (encButtonState == 3)
        {
            stateHandler(&xCtrlFanMutex, &ctrlFan, 3);
        }

        xSemaphoreGive(xEncButtonMutex);
    }
}
