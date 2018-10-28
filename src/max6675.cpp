#include "./../lib/max6675lib.h"
#include <SPI.h>
#include <max6675.h>

int thermoDO1 = 4;
int thermoCS1 = 5;
int thermoCLK1 = 6;

MAX6675 thermocouple;

void max6675Init()
{
    
    thermocouple.begin(thermoCLK1, thermoCS1, thermoDO1);
}

int getByCelsius(){
    return (int)thermocouple.readCelsius();
}
