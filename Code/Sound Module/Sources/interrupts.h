#ifndef INTERRUPTS_HEADER
#define INTERRUPTS_HEADER


// function to initialise SC1
void Init_SC1 (void);

// Timer interrupt definition
__interrupt void SC1_ISR(void);


#endif
