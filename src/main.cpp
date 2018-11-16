#include <Arduino.h>
#include <TimerOne.h>
#include "./../lib/max7219.h"
#include "./../lib/max6675lib.h"
#include "./../lib/encButton.h"
#include "./../lib/encHandlerTask.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t xEncButtonMutex;
SemaphoreHandle_t xCtrlTemp1Mutex;
SemaphoreHandle_t xCtrlTemp2Mutex;
SemaphoreHandle_t xCtrlFanMutex;

int encButtonState = 0;

int tempBtnOnPin1 = A4;
int tempBtnOnPin2 = A5;
int powerPin1 = A1;
int powerPin2 = A3;
int thermopare = A0;
int systemFan = 13;
const int fanPin = 9;

int temperature1 = 0;
int temperature2 = 0;
int ctrlTemp1 = 0;
int ctrlTemp2 = 0;
int tempBtnOnState1 = false;
int tempBtnOnState2 = false;
int ctrlFan = 0;

int get_temperature_period = 250;
unsigned long time_now = 0;

void TaskBlink(void *pvParameters);
void TaskMainDisplay(void *pvParameters);

void setup()
{
    encButtonInit();
    encHandlerTaskInit();

    max7219Init();
    // max6675Init();
    // Timer1.initialize(34); // 40 us = 25 kHz
    // // Timer1.initialize(20); // 40 us = 25 kHz

    // pinMode(tempBtnOnPin1, INPUT);
    // pinMode(tempBtnOnPin2, INPUT);
    // pinMode(powerPin1, OUTPUT);
    // pinMode(powerPin2, OUTPUT);
    // pinMode(thermopare, INPUT);
    // pinMode(systemFan, OUTPUT);
    // pinMode(fanPin, OUTPUT);

    xTaskCreate(
        TaskBlink, (const portCHAR *)"Blink" // A name just for humans
        ,
        128 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);

    xTaskCreate(
        TaskMainDisplay, (const portCHAR *)"TaskMainDisplay" // A name just for humans
        ,
        128 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

void loop()
{

    /**
     * *****FAN CONTROL*****
    */
    // Timer1.pwm(fanPin, ((float)ctrlFan / 100) * 1023);
}

void TaskBlink(void *pvParameters) // This is a task.
{
    (void)pvParameters;
    // initialize digital LED_BUILTIN on pin 13 as an output.
    pinMode(systemFan, OUTPUT);

    for (;;) // A Task shall never return or exit.
    {
        digitalWrite(systemFan, HIGH);         // turn the LED on (HIGH is the voltage level)
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
        digitalWrite(systemFan, LOW);          // turn the LED off by making the voltage LOW
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
    }
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
            
            xSemaphoreTake(xCtrlTemp1Mutex, (TickType_t)10);
            // print(temperature1, 1);
            print(ctrlTemp1, 1);
            xSemaphoreGive(xCtrlTemp1Mutex);

            xSemaphoreTake(xCtrlTemp2Mutex, (TickType_t)10);
            // print(temperature2, 2);
            print(ctrlTemp2, 2);
            xSemaphoreGive(xCtrlTemp2Mutex);

            xSemaphoreTake(xCtrlFanMutex, (TickType_t)10);
            print(ctrlFan, 3);
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
