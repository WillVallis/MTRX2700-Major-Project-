#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER

//convert digit array to 7-seg digit array
void digits_to_seg(int digits[4], unsigned char seg_arr[4]);
//convert integer to digit array
void int_to_digits(int num, int digits[4]);
//delay function
void delay(unsigned int time);
//display integer on 7seg
void display_distance(int distance);
//display error message
void display_error();


#endif