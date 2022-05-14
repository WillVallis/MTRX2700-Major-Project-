#include "sound.h"
#include "music.h"

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"

//caution beep function beeps faster as object gets closer to sensor
void caution_beep(int threshold, int distance){
  char beep_note = 'C';
  char beep_octave = '4';
  char beep[7];
  int delay;
  //get variable to describe how close object is to sensor
  int closeness = (distance * 10) / threshold;
  if (closeness > 9){
    closeness = 10;
  }
  //figure out note delay
  delay = 10 - closeness;
  delay += 48; //convert value to ascii
  //create beep array
  beep[0] = 'm';
  beep[1] = beep_note;
  beep[2] = beep_octave;
  beep[3] = delay;
  beep[4] = beep_note;
  beep[5] = beep_octave;
  beep[6] = delay;
  
  //play beep
  setMusic(beep, 0);
}

//play success beep
void success_beep(){
   char beep1[8] = "mC44C44";
   setMusic(beep1, 0);
}
  
  
  