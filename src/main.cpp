#include <Arduino.h>
#include <Encoder.h>
#include <TimerOne.h>
#include "./../lib/max7219.h"
#include "./../lib/max6675lib.h"

Encoder myEnc(3, 7);
long oldPosition = 0;
int buttonEncState = 0;

const byte buttonEncPin = 2;
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

void button()
{
    if (buttonEncState < 3)
        buttonEncState++;
    else if (buttonEncState > 3)
        buttonEncState = 0;
    else
        buttonEncState++;
}

void setup()
{
    max7219Init();
    max6675Init();
    attachInterrupt(digitalPinToInterrupt(buttonEncPin), button, FALLING);
    Timer1.initialize(34); // 40 us = 25 kHz
    // Timer1.initialize(20); // 40 us = 25 kHz

    pinMode(tempBtnOnPin1, INPUT);
    pinMode(tempBtnOnPin2, INPUT);
    pinMode(powerPin1, OUTPUT);
    pinMode(powerPin2, OUTPUT);
    pinMode(buttonEncPin, INPUT_PULLUP);
    pinMode(thermopare, INPUT);
    pinMode(systemFan, OUTPUT);
    // pinMode(fanPin, OUTPUT);
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

void loop()
{
    /**
     * GET BUTTONS STATE
    */
    tempBtnOnState1 = digitalRead(tempBtnOnPin1);
    tempBtnOnState2 = digitalRead(tempBtnOnPin2);
    /**
     * GET TEMPERATURE BY INTERVAL
    */
    if (millis() > time_now + get_temperature_period)
    {
        time_now = millis();
        temperature1 = (int)(getByCelsius1());
        temperature2 = (int)(getByCelsius2());
    }

    /**
     * MAIN DISPLAY
    */
    print(temperature1, 1);
    print(temperature2, 2);
    print(ctrlFan, 3);

    /**
     * *****MENU*****
    */
    if (buttonEncState == 0)
    {
        while (buttonEncState == 0)
        {
            if(buttonEncState != 0) break;
            /**
     * GET BUTTONS STATE
    */
            tempBtnOnState1 = digitalRead(tempBtnOnPin1);
            tempBtnOnState2 = digitalRead(tempBtnOnPin2);
            /**
     * GET TEMPERATURE BY INTERVAL
    */
            if (millis() > time_now + get_temperature_period)
            {
                time_now = millis();
                temperature1 = (int)(getByCelsius1());
                temperature2 = (int)(getByCelsius2());
            }

            /**
     * MAIN DISPLAY
    */
            print(temperature1, 1);
            print(temperature2, 2);
            print(ctrlFan, 3);

            /**
     * *****TEMPERATURE CONTROL*****
    */
            //if soldering iron ON
            if (tempBtnOnState1)
            {
                if (temperature1 < ctrlTemp1)
                    digitalWrite(powerPin1, HIGH); //on
                if (temperature1 >= ctrlTemp1)
                    digitalWrite(powerPin1, LOW); //of
            }
            //if soldering iron OFF
            if (!tempBtnOnState1)
            {
                digitalWrite(powerPin1, LOW); //of
            }
            //if soldering Hairdryer ON
            if (tempBtnOnState2)
            {
                // enable fan
                if (ctrlFan < 10)
                {
                    ctrlFan = 10;
                    // Timer1.pwm(fanPin, ((float)ctrlFan / 100) * 1023);
                    delay(3000);
                }
                // temperature control
                if (temperature2 < ctrlTemp2)
                    digitalWrite(powerPin2, HIGH); //on
                if (temperature2 >= ctrlTemp2)
                    digitalWrite(powerPin2, LOW); //of
                if (temperature2 > 510)
                {
                    digitalWrite(powerPin2, LOW); //off
                    ctrlFan = 100;
                }
            }
            //if soldering Hairdryer OFF
            if (!tempBtnOnState2)
            {
                digitalWrite(powerPin2, LOW); //off

                if (temperature2 > 400)
                    ctrlFan = 100;
                if (temperature2 > 300)
                    ctrlFan = 80;
                if (temperature2 > 150)
                    ctrlFan = 50;
                if (temperature2 > 100)
                    ctrlFan = 30;
                if (temperature2 > 70)
                    ctrlFan = 20;
                if (temperature2 < 50)
                    ctrlFan = 0;
            }
        }
    }
    if (buttonEncState == 1)
    {
        digitalWrite(powerPin2, LOW); //of
        digitalWrite(powerPin1, LOW); //of
        oldPosition = ctrlTemp1;
        myEnc.write(ctrlTemp1);

        while (buttonEncState == 1)
        {
            ctrlTemp1 = encCtrl(500);
            print(ctrlTemp1, 1);
        }
    }
    if (buttonEncState == 2)
    {
        digitalWrite(powerPin2, LOW); //of
        digitalWrite(powerPin1, LOW); //of
        oldPosition = ctrlTemp2;
        myEnc.write(ctrlTemp2);

        while (buttonEncState == 2)
        {
            ctrlTemp2 = encCtrl(500);
            print(ctrlTemp2, 2);
        }
    }
    if (buttonEncState == 3)
    {
        digitalWrite(powerPin2, LOW); //of
        digitalWrite(powerPin1, LOW); //of
        oldPosition = ctrlFan;
        myEnc.write(ctrlFan);

        while (buttonEncState == 3)
        {
            ctrlFan = encCtrl(100);
            print(ctrlFan, 3);
        }
    }

    /**
     * *****FAN CONTROL*****
    */
    Timer1.pwm(fanPin, ((float)ctrlFan / 100) * 1023);
}
