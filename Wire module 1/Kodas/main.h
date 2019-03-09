#include <18F14K22.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=1000000) 
#use i2c(SLAVE, SDA=PIN_B4, SCL=PIN_B6, address=0x2)

#define green_led PIN_C2
#define red_led   PIN_B5

//Checks if any of the wires are cut, main loop
int8 check_wires(int8 this_checked[], int8 &correct_wires_left);
//Gets the main module LED status and sends it to series variable
void get_led_status(int8 incoming);
//Checks if the correct wire was pulled out and incorrect ones
void check_wire_status(int8 wire_to_cut, int8 wire_cut[]);

//Definition of wires that can be cut/pulled out
#define wire8 PIN_B5
#define wire7 PIN_B7
#define wire6 PIN_C7
#define wire5 PIN_C6
#define wire4 PIN_C3
#define wire3 PIN_C4
#define wire2 PIN_C5
#define wire1 PIN_A5

//definition of led, which turns on, when module is solved
#define defused_led  PIN_C2
