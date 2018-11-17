#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

extern int encButtonState;
const byte buttonEncPin = 2;

void TaskEncButtonHandler(void *pvParameters);
void buttonInterrupt(void);

extern SemaphoreHandle_t xEncButtonMutex;
SemaphoreHandle_t xEncButtonSemaphore = NULL;


void encButtonInit()
{
    pinMode(buttonEncPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonEncPin), buttonInterrupt, FALLING);

    xEncButtonSemaphore = xSemaphoreCreateCounting(10, 0);
    xEncButtonMutex = xSemaphoreCreateMutex();

    xTaskCreate(
        TaskEncButtonHandler, (const portCHAR *)"TaskEncButtonHandler" // A name just for humans
        ,
        128 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

void buttonInterrupt()
{
    xSemaphoreGiveFromISR(xEncButtonSemaphore, NULL);
}

void TaskEncButtonHandler(void *pvParameters) // This is a task.
{
    (void)pvParameters;

    while (1)
    {
        if (xEncButtonSemaphore != NULL)
        {
            if (xSemaphoreTake(xEncButtonSemaphore, (TickType_t)10) == pdTRUE)
            {
                xSemaphoreTake(xEncButtonMutex, (TickType_t)10);

                if (encButtonState < 2)
                    encButtonState++;
                else if (encButtonState > 2)
                    encButtonState = 0;
                else
                    encButtonState++;

                xSemaphoreGive(xEncButtonMutex);
            }
        }
    }
    
}
