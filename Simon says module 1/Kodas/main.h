#include <18F2455.h>
#device ADC=10

#FUSES NOWDT INTRC_IO         //No Watch Dog Timer

#use delay(internal=1000000)
//I2C initialization
#use i2c(SLAVE, SDA=PIN_B0, SCL=PIN_B1, address=0x14) 

#INCLUDE <STDLIB.H>           //Needed for srand()

//Led definitions
#define red_led      PIN_C0  
#define yellow_led   PIN_C1
#define blue_led     PIN_A4
#define green_led    PIN_A6

//Transistor definitions
#define red_tran     PIN_A1
#define yellow_tran  PIN_A3
#define blue_tran    PIN_A5
#define green_tran   PIN_C2

//Led which turns on, when modules is defused
#define defused_led  PIN_B4

//Initializes starting pin values
void init(void);
//Gets the status of the main modules leds
void get_led_status(int8 led_status[]);
//The main game loop
void game(int8 series[]);
//Enable or dissable transistors
void enable_trans(void);
void disable_trans(void);
//Blinks leds to show if something is wrong
void display_wrong(void);
void display_wrong2(void);
//Resets all of the leds to 0
void reset_led(void);
//Checks each button if it was pressed
void check_button_press(int8 series[], int8 turn, int8 &fail_state);
//Blinks the sequence of the led flashes
void blink_this_turn(int8 series[], int8 turn);
//Gets the main module LED status and sends it to series variable
void get_led_status(int8 incoming);



