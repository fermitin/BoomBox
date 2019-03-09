#include <main.h>

int8 data = 0;                                           //data is information to be sent

int8 incoming, state;                                    //incoming is the data from the i2c channel state is direction of data coming or going

int8 hp = -1;                                            //The main modules generated HP
int8 min = -1;                                           //The main modules generated minutes
int8 sec = -1;                                           //The main modules generated seconds
int8 seriess[8] = {-1, -1, -1, -1, -1, -1, -1, -1};       //The main modules generated led sequence
                                                         //We use 8 bits, because I2C channel is 8 bits, even though we only need 4 bits

#INT_SSP 
void i2c_interupt (void){ 
   /*state = i2c_isr_state(); 
    
   if(state < 0x80){                                     //Master is sending data 
      incoming = i2c_read(); 
      if(state == 1){                     
         hp = incoming;
      }
      if(state == 2){                     
         min = incoming;
      }
      if(state == 3){
         sec = incoming;
      }
      if(state == 4){
         get_led_status(incoming);
      }
   }
   
   if (state == 0x80){                                   //Not a recieve nor send byte 
      i2c_write(0);                                      //Dummy send so I2C doesnt lock up
   } 
  */
}


void main() {
   //Initializes starting pin values
   init();
   
   //Generate a series of led flashes
   int8 series[50];
   for(int i = 0; i < 50; i++){
      series[i] = rand() % 4;
   }
   
   enable_trans();
   //output_high(defused_led);
   //output_high(red_led);
   /*setup_adc_ports(NO_ANALOGS);
   enable_trans();
   output_high(red_tran);
   output_high(red_led);*/
   
   while(TRUE){
      //The main game starts
      game(series);
      //The player pressed wrong
      display_wrong();

   }
}

//Gets the main module LED status and sends it to series variable
void get_led_status(int8 incoming){
   int c, k;
   int bin = incoming;
   for (c = 0; c < 8; ++c){
      k = bin >> c;
      if (k & 1){
         seriess[c] = 1;
      }else{
         seriess[c] = 0;
      }
   } 
}


//Initializes starting pin values
void init(void){
   //I2C init interrupts
   enable_interrupts(INT_SSP); 
   enable_interrupts(GLOBAL); 
   
   //Control of leds are set to 0
   output_low(red_tran);
   output_low(yellow_tran);
   output_low(blue_tran);
   output_low(green_tran);
   output_low(red_led);
   output_low(yellow_led);
   output_low(blue_led);
   output_low(green_led);
}

void get_led_status(int8 led_status[]){
   while(true){
      for(int i = 0; i < 4; i++){
         //if(led_status
      }
   }
}

//The main game
void game(int8 series[]){
   //If the player pressed wrong, fail_state = 1, else 0
   int fail_state = 0;
   
   for(int turn = 0; turn < 49; turn++){
      //Blink all of the lights of this turn
      blink_this_turn(series, turn);
   
      //Turn on transistors
      enable_trans();
      
      //Check if the right buttons are pressed
      check_button_press(series, turn, fail_state);
      
      //If pressed incorrectly
      if(fail_state == 1){
         display_wrong2();
         break;
      }
      
      
   }
}

void enable_trans(void){
   output_high(red_tran);
   output_high(yellow_tran);
   output_high(blue_tran);
   output_high(green_tran);
}

void disable_trans(void){
   output_low(red_tran);
   output_low(yellow_tran);
   output_low(blue_tran);
   output_low(green_tran);
}

void display_wrong(void){
   output_high(red_led);
   output_high(green_led);
   output_high(yellow_led);
   output_high(blue_led);
   delay_ms(2000);
   output_low(red_led);
   output_low(green_led);
   output_low(yellow_led);
   output_low(blue_led);
}

void display_wrong2(void){
   for(int i = 0; i < 4; i++){
      output_high(red_led);
      output_high(green_led);
      output_high(yellow_led);
      output_high(blue_led);
      delay_ms(200);
      output_low(red_led);
      output_low(green_led);
      output_low(yellow_led);
      output_low(blue_led);
      delay_ms(200);
   }
}

void check_button_press(int8 series[], int8 turn, int8 &fail_state){
   for(int i = 0; i <= turn; i++){
      //Turn on transistors
      enable_trans();
      
      while(true){
         //Red button
         if(input_state(red_led) == 1){         
            if(series[i] != 0){
               fail_state = 1;
               break;
            }else{
               break;
            }
         }
         
         //Green button
         if(input_state(green_led) == 1){
            if(series[i] != 1){
               fail_state = 1;
               break;
            }else{
               break;
            }
         }
         
         //Blue button
         if(input_state(blue_led) == 1){
            if(series[i] != 2){
               fail_state = 1;
               break;
            }else{
               break;
            }
         }
         
         //Yellow button
         if(input_state(yellow_led) == 1){
            if(series[i] != 3){
               fail_state = 1;
               break;
            }else{
               break;
            }
         }
      }
      
      if(fail_state == 1){
         break;
      }
      
      //Waits for the person to release the buttons
      while(input_state(red_led) == 1 || input_state(green_led) == 1 || input_state(blue_led) == 1 || input_state(yellow_led) == 1){
         
      }
      
      //Turn off transistors
      disable_trans();
      
      delay_ms(200);
   }
}

void blink_this_turn(int8 series[], int8 turn){
   for(int i = 0; i <= turn; i++){
      if(series[i] == 0){
         output_high(red_led);
         delay_ms(500);
         output_low(red_led);
         delay_ms(500);
      }else if(series[i] == 1){
         output_high(green_led);
         delay_ms(500);
         output_low(green_led);
         delay_ms(500);
      }else if(series[i] == 2){
         output_high(blue_led);
         delay_ms(500);
         output_low(blue_led);
         delay_ms(500);
      }else if(series[i] == 3){
         output_high(yellow_led);
         delay_ms(500);
         output_low(yellow_led);
         delay_ms(500);
      }
   }
}

void reset_led(void){
   output_low(red_led);
   output_low(yellow_led);
   output_low(blue_led);
   output_low(green_led);
}
