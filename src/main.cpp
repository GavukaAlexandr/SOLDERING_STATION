#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "./../lib/encButtonTask.h"
#include "./../lib/encHandlerTask.h"
#include "./../lib/displayTask.h"
#include "./../lib/getTemperatureTask.h"
#include "./../lib/controlHotAirSolderingTask.h"


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
    controlHotAirSolderingTaskInit();

    // pinMode(tempBtnOnPin1, INPUT);
    // pinMode(powerPin1, OUTPUT);
    // pinMode(thermopare, INPUT);
    // pinMode(systemFan, OUTPUT);
    // pinMode(fanPin, OUTPUT);
}

void loop() {}
