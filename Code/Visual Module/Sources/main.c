#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "display.h"

//presently configured for hardware demonstration
void main(void) {
  /* put your own code here */
   int num = 7234;
   
   //int digits[4];
   //unsigned char seg_arr[4];
   //int_to_digits(num, digits);
   //digits_to_seg(digits, seg_arr);
   
   while(1){
   display_distance(num);
   }

	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}