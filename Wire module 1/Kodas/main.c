//I2C SLAVE
//Laidu modulis
#include <main.h>

int8 data = 0;                                           //data is information to be sent

int8 incoming, state;                                    //incoming is the data from the i2c channel state is direction of data coming or going

int8 hp = -1;                                            //The main modules generated HP
int8 min = -1;                                           //The main modules generated minutes
int8 sec = -1;                                           //The main modules generated seconds
int8 series[8] = {-1, -1, -1, -1, -1, -1, -1, -1};       //The main modules generated led sequence
                                                         //We use 8 bits, because I2C channel is 8 bits, even though we only need 4 bits

#INT_SSP 
void i2c_interupt (void){ 
   state = i2c_isr_state(); 
    
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
  
}


void main() {
   //I2C init interrupts
   enable_interrupts(INT_SSP); 
   enable_interrupts(GLOBAL); 

   int8 this_checked[99];
   for(int i = 0; i < 99; i++){
      this_checked[i] = 0;
   }
   
   int8 correct_wires_left = 2;            //How many wires left to cut
   
   while(TRUE){
      check_wires(this_checked, correct_wires_left);
   }

}

int8 check_wires(int8 this_checked[], int8 &correct_wires_left){
   int8 wire_cut[6] = {0, 0, 0, 0, 0, 0};  //Boolean if the wire was cut
   
   //If HP is 2
   if(hp == 2 && correct_wires_left >= 1){
      //First Led, orange wire
      if(series[7] == 1 && this_checked[0] == 0){
         check_wire_status(0, wire_cut);
         correct_wires_left -= 1;
         this_checked[0] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //Second Led, violet wire
      if(series[6] == 1 && this_checked[1] == 0){
         check_wire_status(5, wire_cut);
         correct_wires_left -= 1;
         this_checked[1] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //3 and 4 Leds, brown wire
      if(series[5] == 1 && series[4] == 1 && this_checked[2] == 0){
         check_wire_status(4, wire_cut);
         correct_wires_left -= 1;
         this_checked[2] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //3 or 4 Leds, grey wire
      if((series[5] == 1 || series[4] == 1) && this_checked[3] == 0){
         check_wire_status(3, wire_cut);
         correct_wires_left -= 1;
         this_checked[3] = 1;
         output_high(defused_led);
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //Other, yellow wire
      if(this_checked[4] == 0){
         check_wire_status(1, wire_cut);
         correct_wires_left -= 1;
         this_checked[4] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //If all else fails, black wire
      if(this_checked[5] == 0){
         check_wire_status(2, wire_cut);
         correct_wires_left -= 1;
         this_checked[5] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
   }
   
   //If HP is 1
   if(hp == 1 && correct_wires_left >= 1){
      //First Led, brown wire
      if(series[7] == 1 && this_checked[6] == 0){
         check_wire_status(4, wire_cut);
         correct_wires_left -= 1;
         this_checked[6] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //Second Led, yellow wire
      if(series[6] == 1 && this_checked[7] == 0){
         check_wire_status(1, wire_cut);
         correct_wires_left -= 1;
         this_checked[7] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //3 and 4 Leds, black wire
      if(series[5] == 1 && series[4] == 1 && this_checked[8] == 0){
         check_wire_status(2, wire_cut);
         correct_wires_left -= 1;
         this_checked[8] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //3 or 4 Leds, violet wire
      if((series[5] == 1 || series[4] == 1) && this_checked[9] == 0){
         check_wire_status(5, wire_cut);
         correct_wires_left -= 1;
         this_checked[9] = 1;
         output_high(defused_led);
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //Other, orange wire
      if(this_checked[10] == 0){
         check_wire_status(0, wire_cut);
         correct_wires_left -= 1;
         this_checked[10] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
      //If all else fails, grey wire
      if(this_checked[11] == 0){
         check_wire_status(3, wire_cut);
         correct_wires_left -= 1;
         this_checked[11] = 1;
         if(correct_wires_left <= 0){
            return 0;
         }
      }
   }
   
   if(correct_wires_left <= 0){
      output_high(defused_led);
   }
}

//Gets the main module LED status and sends it to series variable
void get_led_status(int8 incoming){
   int c, k;
   int bin = incoming;
   for (c = 0; c < 8; ++c){
      k = bin >> c;
      if (k & 1){
         series[c] = 1;
      }else{
         series[c] = 0;
      }
   } 
}

//Checks if the correct wire was pulled out and incorrect ones
void check_wire_status(int8 wire_to_cut, int8 wire_cut[]){
   int8 correct_wire_cut = 0;   //Equals 1 if the correct wire was cut

   //Checks if the correct wire was already cut
   for(int i = 0; i < 6; i++){
      if(wire_to_cut == i && wire_cut[i] == 1){
         correct_wire_cut = 1;
      }
   }
   
   while(correct_wire_cut == 0){
      //Checks if the correct wire was cut
      if(wire_to_cut == 0 && wire_cut[0] == 0 && input_state(wire1) == 0){
         wire_cut[0] = 1;
         correct_wire_cut = 1;
         break;
      }else if(wire_to_cut == 1 && wire_cut[1] == 0 && input_state(wire2) == 0){
         wire_cut[1] = 1;
         correct_wire_cut = 1;
         break;
      }else if(wire_to_cut == 2 && wire_cut[2] == 0 && input_state(wire3) == 0){
         wire_cut[2] = 1;
         correct_wire_cut = 1;
         break;
      }else if(wire_to_cut == 3 && wire_cut[3] == 0 && input_state(wire4) == 0){
         wire_cut[3] = 1;
         correct_wire_cut = 1;
         break;
      }else if(wire_to_cut == 4 && wire_cut[4] == 0 && input_state(wire5) == 0){
         wire_cut[4] = 1;
         correct_wire_cut = 1;
         break;
      }else if(wire_to_cut == 5 && wire_cut[5] == 0 && input_state(wire6) == 0){
         wire_cut[5] = 1;
         correct_wire_cut = 1;
         break;
      }
      
      //checks if the wrong wire was cut
      else if(wire_to_cut != 0 && wire_cut[0] == 0 && input_state(wire1) == 0){
         //Decrease hp
         wire_cut[0] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }else if(wire_to_cut != 1 && wire_cut[1] == 0 && input_state(wire2) == 0){
         //Decrease hp
         wire_cut[1] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }else if(wire_to_cut != 2 && wire_cut[2] == 0 && input_state(wire3) == 0){
         //Decrease hp
         wire_cut[2] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }else if(wire_to_cut != 3 && wire_cut[3] == 0 && input_state(wire4) == 0){
         //Decrease hp
         wire_cut[3] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }else if(wire_to_cut != 4 && wire_cut[4] == 0 && input_state(wire5) == 0){
         //Decrease hp
         wire_cut[4] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }else if(wire_to_cut != 5 && wire_cut[5] == 0 && input_state(wire6) == 0){
         //Decrease hp
         wire_cut[5] = 1;
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
         output_high(defused_led);
         delay_ms(500);
         output_low(defused_led);
         delay_ms(500);
      }
   }
}
