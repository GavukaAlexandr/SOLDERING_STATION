#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <TimerOne.h>
#include <digitalWriteFast.h>

const int tempBtnOnPin2 = A5;
const int powerPin2 = A3;
const int fanPin = 9;

extern int temperature2;
extern int ctrlTemp2;
extern int ctrlFan;

extern SemaphoreHandle_t xTemperature1Mutex;
extern SemaphoreHandle_t xTemperature2Mutex;
extern SemaphoreHandle_t xCtrlFanMutex;

void TaskControlHotAirSoldering(void *pvParameters);

void controlHotAirSolderingTaskInit()
{
    pinMode(tempBtnOnPin2, INPUT);
    pinModeFast(powerPin2, OUTPUT);
    pinModeFast(13, OUTPUT);

    Timer1.initialize(34); // 40 us = 25 kHz

    xTaskCreate(
        TaskControlHotAirSoldering, (const portCHAR *)"TaskControlHotAirSoldering" // A name just for humans
        ,
        128 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
}

void TaskControlHotAirSoldering(void *pvParameters) // This is a task.
{
    (void)pvParameters;

    while (1)
    {
        xSemaphoreTake(xTemperature2Mutex, (TickType_t)10);
        xSemaphoreTake(xCtrlFanMutex, (TickType_t)10);
        Timer1.pwm(fanPin, ((float)ctrlFan / 100) * 1023);
        int delayAfterOff = 118;
        if (digitalRead(tempBtnOnPin2))
        {
            if (ctrlFan < 15)
                ctrlFan = 20;
            if (temperature2 < ctrlTemp2)
            {
                digitalWriteFast(powerPin2, HIGH); //on
                digitalWriteFast(13, HIGH);        //on

                if (ctrlTemp2 > 299)
                {
                    delayAfterOff = 0;
                    if(ctrlFan < 16) delayMicroseconds(300);
                    if(ctrlFan > 19) delayMicroseconds(100);
                }
                else if (ctrlTemp2 > 199)
                {
                    delayAfterOff = 70;
                    if(ctrlFan < 16) delayMicroseconds(600);
                    if(ctrlFan > 19)
                    {
                        delayAfterOff = 60;
                        delayMicroseconds(600);
                    } 
                    if(ctrlFan > 30) delayMicroseconds(600);
                    if(ctrlFan > 49)
                    {
                        delayAfterOff = 10;
                        delayMicroseconds(300);
                    }
                }
                else if (ctrlTemp2 > 90)
                {
                    if(ctrlFan < 16) delayMicroseconds(30);
                    if(ctrlFan > 19) delayMicroseconds(200);
                    if(ctrlFan > 29) delayMicroseconds(250);
                    if(ctrlFan > 50) delayMicroseconds(300);
                    if(ctrlFan > 90) delayMicroseconds(350);
                }
                else if (ctrlTemp2 > 49){
                    if(ctrlFan < 16) delayMicroseconds(30);
                    if(ctrlFan > 19) delayMicroseconds(30);
                }

                digitalWriteFast(powerPin2, LOW); //off
                digitalWriteFast(13, LOW);        //on
                if (delayAfterOff) vTaskDelay((int)map(ctrlFan, 0, 100, delayAfterOff, 0) / portTICK_PERIOD_MS);
                else if (ctrlTemp2 < 350 && !delayAfterOff) vTaskDelay(1 / portTICK_PERIOD_MS);
                else  delayMicroseconds(1500);
            }

            else
            {
                digitalWriteFast(powerPin2, LOW); //off
            }
        }
        else
        {
            digitalWriteFast(powerPin2, LOW); //off
            if (temperature2 > 50)
            {
                ctrlFan = 100;
            }
            else
            {
                ctrlFan = 0;
            }
        }

        xSemaphoreGive(xCtrlFanMutex);
        xSemaphoreGive(xTemperature2Mutex);
    }
}
