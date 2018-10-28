#include <Arduino.h>
#include <Encoder.h>
#include <TimerOne.h>
#include "./../lib/max7219.h"
#include "./../lib/max6675lib.h"

Encoder myEnc(3, 7);
long oldPosition = 0;
const byte buttonPin = 2;
int buttonState = 0;
const int fanPin = 9;


void button()
{
    delay(50);
    if(buttonState < 2) buttonState++;
    else if(buttonState > 2) buttonState = 0;
    else buttonState++;
}

void setup()
{
    max7219Init();
    max6675Init();
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), button, FALLING);
    Timer1.initialize(40);  // 40 us = 25 kHz
}

void loop()
{
    printLCD1(getByCelsius());
    delay(175);

    long newPosition = myEnc.read();
    if (newPosition != oldPosition)
    {
        if(newPosition < 0) myEnc.write(0);
        if(newPosition > 500) myEnc.write(500);
        oldPosition = newPosition;
    }
    printLCD2(newPosition);

    Timer1.pwm(fanPin, ((float)oldPosition / 100) * 1023);
}
