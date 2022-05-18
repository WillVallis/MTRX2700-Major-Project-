#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "music.h"
#include "sound.h"
#include "timer.h"
#include "display.h"


void main(void) {
  /* put your own code here */
    /* char beep1[8] = "mC44C44";
     setMusic(beep1, 0);*/
     int threshold = 100;
     int distance = 0;
     int *wait;
     int wait_time = 1;
     //caution_beep(threshold, distance);
      // Initialise our modules
  initTimer(); 
  initMusic();

	EnableInterrupts;
	//function would theorectically work with the commented out lines in practice
  while(distance <= threshold){
    caution_beep(threshold, distance, &wait);
    //wait_time = *wait;
    //delay(wait_time);
    distance += 1;
    } 
  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
