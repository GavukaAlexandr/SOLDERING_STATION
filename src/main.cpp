#include <Arduino.h>
#include "./../lib/max7219.h"

void setup()
{
 max7219Init();
}

void loop()
{

    for (int c = 0; c < 9999; c++)
    {
        printLCD1(c);
        // delay(10);
        printLCD2(c);
        delay(50);
    }
}
