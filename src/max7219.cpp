#include "./../lib/max7219.h"
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(12, 11, 10, 1);
int digitBuffer[4] = {0};

/* we always wait a bit between updates of the display */
unsigned long delaytime = 250;

void max7219Init()
{
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
    lc.shutdown(0, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0, 8);
    /* and clear the display */
    lc.clearDisplay(0);
}

void prepareDigits(int data)
{
    // digitBuffer[3] = int(data) / 1000;
    digitBuffer[2] = (int(data) % 1000) / 100;
    digitBuffer[1] = (int(data) % 100) / 10;
    digitBuffer[0] = (int(data) % 100) % 10;
}

#define CLEAR_INTERVAL 1000
unsigned long time_now_clear = 0;

/**
 * @data = int for print to LCD
 * @lcdType = part LCD 1, 2 or 3
 * 1 = xxx*****
 * 2 = ***xxx**
 * 3 = ******xx
*/
void print(int data, int lcdType, bool blink)
{
    prepareDigits(data);

    if (blink)
    {
        lc.clearDisplay(0);
        delay(500);
    }

    if (millis() > time_now_clear + CLEAR_INTERVAL)
    {
        time_now_clear = millis();
        lc.clearDisplay(0);
    }

    if (lcdType == 1)
        lcdType = 5;
    if (lcdType == 2)
        lcdType = 2;
    if (lcdType == 3)
        lcdType = 0;

    int digitCounts = 1;
    if (data > 9)
        digitCounts = 2;
    if (data > 99)
        digitCounts = 3;

    for (size_t i = 0; i < digitCounts; i++)
    {
        lc.setDigit(0, i + lcdType, digitBuffer[i], false);
    }
}
