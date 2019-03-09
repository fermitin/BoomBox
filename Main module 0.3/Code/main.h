#include <18F24K22.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=1000000)
//I2C initialization
#use i2c(Master, sda=PIN_C4, scl=PIN_C3) 

#INCLUDE <STDLIB.H>           //Needed for srand()

//First module address for I2C
#define SLAVE1_WRT_ADDR   0x2 
#define SLAVE1_READ_ADDR  0x3 

//Second modules addres for I2C
#define SLAVE2_WRT_ADDR   0x14 
#define SLAVE2_READ_ADDR  0x15 

#define shclk   PIN_B2     //74HC595 Shift clock
#define strclk  PIN_B1     //74HC595 Store clock
#define ser4    PIN_B0     //74HC595 Data pin of segment 1
#define ser3    PIN_B3     //74HC595 Data pin of segment 2
#define ser2    PIN_B4     //74HC595 Data pin of segment 3
#define ser1    PIN_B5     //74HC595 Data pin of segment 4
#define ser5    PIN_C5     //74HC595 Data pin of segment 5
#define snd1    PIN_A0     //Buzzer contact 1
#define snd2    PIN_A1     //Buzzer contact 2
#define led1    PIN_A2     //Debug led, one of 4 yellow
#define led2    PIN_A3     //Debug led, one of 4 green
#define led3    PIN_A4     //Debug led, one of 4 blue
#define led4    PIN_A5     //Debug led, one of 4 

//Resets all of the 7-segments to zero
void reset(void);
//Starts calculating the digits for the 7-segments
void display_loop(int8 &min, int8 &sec, int8 &health);
//Seperates the numbers, ex 23 -> 2 and 3
void seperateNumbers(int8 &min1, int8 &min2, int8 &sec1, int8 &sec2, int8 min, int8 sec);
//Sets the needed 7-segment's digit
void display(int8 digit, int8 data_pin, int8 segment);
//Main game check if the player failed
int8 check_fail_state(int8 &min, int8 &sec, int8 health);
//Beeping sound generation
void beep_sound(int8 min, int8 sec);
//Initialize the system
void init(void);
//Generation of the seed
void seed_generation(void);
//Set debug leds randomly
void led_series_set(char series[]);
//Sends all of the values to other modules using I2C
void send_gen_values(char series[], int8 health, int8 min, int8 sec);
//Checks if HP is reduced from modules
void recieve_hp_data(int8 &health, int8 &modules_left);

//Invoke, to display the number on the 7 segment, that is in the name of var.
int8 disp0[] = {0,0,1,1,1,1,1,1};
int8 disp1[] = {0,0,0,0,0,1,1,0};
int8 disp2[] = {0,1,0,1,1,0,1,1};
int8 disp3[] = {0,1,0,0,1,1,1,1};
int8 disp4[] = {0,1,1,0,0,1,1,0};
int8 disp5[] = {0,1,1,0,1,1,0,1};
int8 disp6[] = {0,1,1,1,1,1,0,1};
int8 disp7[] = {0,0,0,0,0,1,1,1};
int8 disp8[] = {0,1,1,1,1,1,1,1};
int8 disp9[] = {0,1,1,0,1,1,1,1};

//IMPROVEMENTS
//led3 and led4 bigger value resistors.
//timer vidini
