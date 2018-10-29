#include <Arduino.h>
#include <Encoder.h>
#include <TimerOne.h>
#include "./../lib/max7219.h"
#include "./../lib/max6675lib.h"

Encoder myEnc(3, 7);
long oldPosition = 0;
const byte buttonPin = 2;
int buttonState = 0;
int ctrlTemp1 = 0;
int ctrlTemp2 = 0;
int ctrlFan = 0;
const int fanPin = 9;

int temperature1 = 0;
int temperature2 = 0;

void button()
{
    if (buttonState < 3)
        buttonState++;
    else if (buttonState > 3)
        buttonState = 0;
    else
        buttonState++;
}

void setup()
{
    max7219Init();
    max6675Init();
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), button, FALLING);
    Timer1.initialize(34); // 40 us = 25 kHz
}

int get_temperature_period = 250;
unsigned long time_now = 0;

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
    if (millis() > time_now + get_temperature_period)
    {
        time_now = millis();
        temperature1 = (int)(getByCelsius1());
        temperature2 = (int)(getByCelsius2());
    }

    print(temperature1, 1);
    print(temperature2, 2);
    // print(10, 3);

    if (buttonState == 1)
    {
        oldPosition = ctrlTemp1;
        myEnc.write(ctrlTemp1);

        while (buttonState == 1)
        {
            ctrlTemp1 = encCtrl(500);
            print(ctrlTemp1, 1);
        }
    }
    else if (buttonState == 2)
    {
        oldPosition = ctrlTemp2;
        myEnc.write(ctrlTemp2);

        while (buttonState == 2)
        {
            ctrlTemp2 = encCtrl(500);
            print(ctrlTemp2, 2);
        }
    }

    else if (buttonState == 3)
    {
        /* code */
    }

    // print(newPosition, 3);

    // myEnc.write(30);

    Timer1.pwm(fanPin, ((float)oldPosition / 100) * 1023);
}
