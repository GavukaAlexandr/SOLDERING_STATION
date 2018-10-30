#include <Arduino.h>
#include <Encoder.h>
#include <TimerOne.h>
#include "./../lib/max7219.h"
#include "./../lib/max6675lib.h"

Encoder myEnc(3, 7);
long oldPosition = 0;
const byte buttonPin = 2;
int buttonState = 0;
int ctrlFan = 0;
const int fanPin = 9;

int get_temperature_period = 250;
unsigned long time_now = 0;

int temperature1 = 0;
int temperature2 = 0;
int ctrlTemp1 = 0;
int ctrlTemp2 = 0;
int tempBtnOnState1 = false;
int tempBtnOnState2 = false;
int tempBtnOnPin1 = A0;
int tempBtnOnPin2 = A1;
int powerPin1 = A2;
int powerPin2 = A3;

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
    pinMode(tempBtnOn1, INPUT_PULLUP);
    pinMode(tempBtnOn2, INPUT_PULLUP);
    pinMode(powerPin1,  OUTPUT);
    pinMode(powerPin2,  OUTPUT);
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
    tempBtnOnState1 = digitalRead(tempBtnOn1);
    tempBtnOnState2 = digitalRead(tempBtnOn2);
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
    if(tempBtnOnState1){
        if(temperature1 < ctrlTemp1) digitalWrite(powerPin1, HIGH); //on
        if(temperature1 > ctrlTemp1) digitalWrite(powerPin1, LOW); //of
    }
    //if soldering iron OFF
    if(!tempBtnOnState1){
        digitalWrite(powerPin1, LOW); //of
    }
    //if soldering Hairdryer ON
    else if(tempBtnOnState2){
        if(temperature2 < ctrlTemp2) digitalWrite(powerPin2, HIGH) //on
        if(temperature2 > ctrlTemp2) digitalWrite(powerPin2, LOW) //of
        if(temperature2 > 510){
            digitalWrite(powerPin2, LOW) //off
            ctrlFan = 100;
        }
    }
    //if soldering Hairdryer OFF
    else if (!tempBtnOnState2)
    {
        if(temperature2 > 400) ctrlFan = 100;
        if(temperature2 > 300) ctrlFan = 80;
        if(temperature2 > 150) ctrlFan = 50;
        if(temperature2 > 100) ctrlFan = 30;
        if(temperature2 > 70) ctrlFan = 20;
        if(temperature2 < 50) ctrlFan = 0;
    }
    
    /**
     * *****MENU*****
    */
    // if (buttonState == 0)
    // {
    // }
    /* else  */if (buttonState == 1)
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
        oldPosition = ctrlFan;
        myEnc.write(ctrlFan);

        while (buttonState == 3)
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
