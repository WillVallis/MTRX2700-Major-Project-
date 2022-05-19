#include "timer.h"

#include "derivative.h"


void initTimer() {
    TSCR2 = 7; // Set prescaler to max
    TSCR1 |= TSCR1_TEN_MASK + TSCR1_TFFCA_MASK; // Enable timer and set fast timer flag clear to auto clear flag
}

void enableOutputCompare(int channel) {
    TIOS |= 1 << channel; // Enable output compare for specified channel
    TIE |= 1 << channel; // Enable interrupts for specified channel
}
