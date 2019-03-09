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
   
   while(TRUE){
      if(sec == 30 || sec == 59){
         output_high(green_led);
      }else{
         output_low(green_led);
      }
   }

}

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
