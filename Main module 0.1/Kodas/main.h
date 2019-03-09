#include <18F14K22.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=1000000) 
#use i2c(SLAVE, SDA=PIN_B4, SCL=PIN_B6, address=0x14)

#define green_led PIN_A4

void get_led_status(int8 incoming);
