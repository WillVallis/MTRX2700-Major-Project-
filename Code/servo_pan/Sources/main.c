#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>

#include "servo.h"


void main(void) {

  
  #ifndef SIMULATION_TESTING

  // make sure the board is set to 24MHz
  //  this is needed only when not using the debugger
  PLL_Init();

  // initialise PWM
  PWMinitialise();
  setServoPose(100, 100);

  #endif
  

  Init_TC6();
  
	EnableInterrupts;
  //COPCTL = 7;
  _DISABLE_COP();
    
  for(;;) {
  
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
