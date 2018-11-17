#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "./../lib/encButtonTask.h"
#include "./../lib/encHandlerTask.h"
#include "./../lib/displayTask.h"
#include "./../lib/getTemperatureTask.h"
// #include "./../lib/controlHotAirSolderingTask.h"


SemaphoreHandle_t xEncButtonMutex;
SemaphoreHandle_t xCtrlTemp1Mutex;
SemaphoreHandle_t xCtrlTemp2Mutex;
SemaphoreHandle_t xCtrlFanMutex;

SemaphoreHandle_t xTemperature1Mutex;
SemaphoreHandle_t xTemperature2Mutex;

int encButtonState = 0;

const int tempBtnOnPin1 = A4;
const int powerPin1 = A1;
const int thermoparePin = A0;
const int systemFanPin = 13;


int temperature1 = 0;
int temperature2 = 0;
int ctrlTemp1 = 0;
int ctrlTemp2 = 0;
// int tempBtnOnState1 = false;
// int tempBtnOnState2 = false;
int ctrlFan = 0;

int get_temperature_period = 250;
unsigned long time_now = 0;

// void TaskBlink(void *pvParameters);

void setup()
{
    encButtonInit();
    encHandlerTaskInit();
    displayTaskInit();
    getTemperatureTaskInit();
    // controlHotAirSolderingTaskInit();
    
        // Timer1.initialize(34); // 40 us = 25 kHz
    // // Timer1.initialize(20); // 40 us = 25 kHz

    // pinMode(tempBtnOnPin1, INPUT);
    // pinMode(powerPin1, OUTPUT);
    // pinMode(thermopare, INPUT);
    // pinMode(systemFan, OUTPUT);
    // pinMode(fanPin, OUTPUT);

    // xTaskCreate(
    //     TaskBlink, (const portCHAR *)"Blink" // A name just for humans
    //     ,
    //     45 // This stack size can be checked & adjusted by reading the Stack Highwater
    //     ,
    //     NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    //     ,
    //     NULL);
}

void loop()
{}

// void TaskBlink(void *pvParameters) // This is a task.
// {
//     (void)pvParameters;
//     // initialize digital LED_BUILTIN on pin 13 as an output.
//     pinMode(systemFanPin, OUTPUT);

//     for (;;) // A Task shall never return or exit.
//     {
//         digitalWrite(systemFanPin, HIGH);         // turn the LED on (HIGH is the voltage level)
//         vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
//         digitalWrite(systemFanPin, LOW);          // turn the LED off by making the voltage LOW
//         vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
//     }
// }
