#include "./../lib/max6675lib.h"
#include <SPI.h>
#include <max6675.h>

int thermoDO = 4;
int thermoCLK = 6;
int thermoCS1 = 5;
int thermoCS2 = 8;

MAX6675 thermocouple11;
MAX6675 thermocouple12;

void max6675Init()
{
    
    thermocouple11.begin(thermoCLK, thermoCS1, thermoDO);
    thermocouple12.begin(thermoCLK, thermoCS2, thermoDO);
}

double getByCelsius1(){
    return thermocouple11.readCelsius();
}

double getByCelsius2(){
    return thermocouple12.readCelsius();
}
