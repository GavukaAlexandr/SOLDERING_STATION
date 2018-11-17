// #include <Arduino.h>
// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>
// #include <TimerOne.h>

// const int tempBtnOnPin2 = A5;
// const int powerPin2 = A3;
// const int fanPin = 9;

// extern int temperature2;
// extern int ctrlTemp2;
// extern int ctrlFan;

// extern SemaphoreHandle_t xTemperature1Mutex;
// extern SemaphoreHandle_t xTemperature2Mutex;
// extern SemaphoreHandle_t xCtrlFanMutex;

// void TaskControlHotAirSoldering(void *pvParameters);

// void controlHotAirSolderingTaskInit()
// {
//     pinMode(tempBtnOnPin2, INPUT);
//     pinMode(powerPin2, OUTPUT);

//     Timer1.initialize(34); // 40 us = 25 kHz

//     xTaskCreate(
//         TaskControlHotAirSoldering, (const portCHAR *)"TaskControlHotAirSoldering" // A name just for humans
//         ,
//         128 // This stack size can be checked & adjusted by reading the Stack Highwater
//         ,
//         NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
//         ,
//         NULL);
// }

// void TaskControlHotAirSoldering(void *pvParameters) // This is a task.
// {
//     (void)pvParameters;

//     while (1)
//     {
//         xSemaphoreTake(xTemperature2Mutex, (TickType_t)10);
//         xSemaphoreTake(xCtrlFanMutex, (TickType_t)10);
//         Timer1.pwm(fanPin, ((float)ctrlFan / 100) * 1023);

//         if (digitalRead(tempBtnOnPin2))
//         {

//             if (ctrlFan < 15)
//                 ctrlFan = 15;
//             if (temperature2 < ctrlTemp2)
//             {
//                 digitalWrite(powerPin2, HIGH); //on
//                 vTaskDelay(5 / portTICK_PERIOD_MS);
//                 digitalWrite(powerPin2, LOW); //off
//             }
//             else
//             {
//                 digitalWrite(powerPin2, LOW); //off
//             }
//         }
//         else
//         {
//             digitalWrite(powerPin2, LOW); //off
//             if (temperature2 > 50)
//             {
//                 ctrlFan = 50;
//             }
//             else
//             {
//                 ctrlFan = 0;
//             }
//         }

//         xSemaphoreGive(xCtrlFanMutex);
//         xSemaphoreGive(xTemperature2Mutex);
//     }
//     // vTaskDelay(50 / portTICK_PERIOD_MS);
// }
