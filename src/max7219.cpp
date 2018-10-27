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

void printLCD1(int data)
{
    digitBuffer[3] = int(data) / 1000;
    digitBuffer[2] = (int(data) % 1000) / 100;
    digitBuffer[1] = (int(data) % 100) / 10;
    digitBuffer[0] = (int(data) % 100) % 10;

    for (size_t i = 0; i < 4; i++)
    {
        lc.setDigit(0, i + 4, digitBuffer[i], false);
    }
}

void printLCD2(int data)
{
    digitBuffer[3] = int(data) / 1000;
    digitBuffer[2] = (int(data) % 1000) / 100;
    digitBuffer[1] = (int(data) % 100) / 10;
    digitBuffer[0] = (int(data) % 100) % 10;

    for (size_t i = 0; i < 4; i++)
    {
        lc.setDigit(0, i, digitBuffer[i], false);
    }
}
