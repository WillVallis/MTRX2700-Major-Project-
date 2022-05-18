//need this to configure ports
#include <mc9s12dp256.h>        /* derivative information */

#include "display.h"
//this array contains the 7-seg conversions for each digit 0-9
unsigned char SegPat[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//this array contains the values configured to each of the 4 7-seg displays
unsigned char display_seg[4] = {0xFE, 0xFD, 0xFB, 0xF7};

//int to digits function
void int_to_digits(int num, int digits[4]){
//fill array with 0
  int count = 0;
  int divider = 1;
  while(count < 4){
    digits[count] = 0;
    count++;
  }
 //check number of digits. assumed value won't be greater than 9999
 //add digits to array
  if(num < 9){
    count = 3;
    digits[count] = num;
  }
   
  else if(num > 9 && num <= 99){
    count = 2;
    digits[count] = num / 10;
    digits[count + 1] = num % 10;
  }
  
  else if(num > 99 && num <= 999){
    count = 1;   
    divider = 100;  
    while(count < 4){
      digits[count] = num / divider;
      num %= divider;
      divider /= 10;
      count++;
    }
  } 
  
  else{
   count = 0;
   divider = 1000;
   while(count < 4){
    digits[count] = num / divider;
    num %= divider;
    divider /= 10;
    count++;
   }
  }
  }
  
 //digit array to 7-seg array
  void digits_to_seg(int digits[4], unsigned char seg_arr[4]){
    int count = 0;
    int compare = 0;
    //compare digit to value, if they match, it is that index in the 7 seg array
    while(count < 4){
      compare = 0;
      while(compare < 10){
        if(digits[count] == compare){
           seg_arr[count] = SegPat[compare];
           count++;
           compare = 0;
        } 
        else{
          compare++;
        }
      }
      count++;
    }
  }
  
// Delay function
void delay(unsigned int time) {
    int count_one;
    int count_two;  
   
    //time *= 5;
    for(count_one = 0;count_one < time;count_one++)
      for(count_two = 0;count_two < 4000;count_two++);
}
  
  //display on 7 seg 4 digit integer
  void display_distance(int distance){
    //variables
    int count = 0;
    unsigned char seg_arr[4];
    int digits[4];
    unsigned int time = 1;
     
    //convert distance to digit array
    int_to_digits(distance, digits);
    
    //convert digit array to 7 seg value array   
    digits_to_seg(digits, seg_arr);
    
    //configure ports for output
    DDRB = 0xFF;
    DDRP = 0xFF;
    
    //display values on 7-segs
    while(count < 4){
      PORTB = seg_arr[count];
      PTP = display_seg[count];
      delay(time);
      count++; 
    }   
    /*//for sim testing
    PORTA = seg_arr[0];   //0x00
    PORTB = seg_arr[1];   //0x01
    PORTE = seg_arr[2];   //0x08
    PORTK = seg_arr[3];   //0x32
    */
  }
  
  //use 7-segs to display error message
  void display_error(){
    //set time variable
    int time = 1;
    //create error message array, E, r, O
    unsigned char error_msg[3] = {0x79, 0x50, 0x3F};
    //configure ports for output
    DDRB = 0xFF;
    DDRP = 0xFF;
    //send letter to port B, send 7seg display to port P
    //delay, repeat for each letter
    PORTB = error_msg[0];
    PTP = display_seg[0];
    delay(time);
    PORTB = error_msg[1];
    PTP = display_seg[1];
    delay(time);
    PORTB = error_msg[2];
    PTP = display_seg[2];
    delay(time);
    PORTB = error_msg[1];
    PTP = display_seg[3];
    delay(time);
  }
    
    
    
  