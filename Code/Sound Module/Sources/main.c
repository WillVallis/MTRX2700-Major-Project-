#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "music.h"
#include "sound.h"
#include "timer.h"
#include "display.h"


void main(void) {
  //set variables
  int threshold = 100;
  int distance = 0;
  int *wait;
  int wait_time = 1;
  
  // Initialise our modules
  initTimer(); 
  initMusic();

	EnableInterrupts;
	/*function would theorectically work by using function to repeatedly 
	get distance as well as the other measurements
	then there would be an if statement calling function if threshold is met*/
	
	//to demonstrate function
  while(distance <= threshold){
    caution_beep(threshold, distance, &wait);
    distance += 1;
    } 
  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
