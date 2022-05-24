#include "sound.h"
#include "music.h"
//#include "display.h"

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"

//caution beep function beeps faster as object gets closer to sensor
void caution_beep(int threshold, int distance, int *wait){
  char beep_note = 'C';
  char beep_octave = '4';
  char beep[7];
  int note_delay;
  //int wait;
  //get variable to describe how close object is to sensor
  int closeness = (distance * 10) / threshold;
  if (closeness > 9){
    closeness = 10;
  }
  //figure out note delay
  note_delay = 10 - closeness;
  if(note_delay > 4){
    *wait = 700 / (2 * note_delay);
  } 
  else if(note_delay <= 4){
    *wait = 160;
  }
   else if(note_delay <= 2){
    *wait = 80;
  }
  note_delay += 48; //convert value to ascii
  //create beep array
  beep[0] = 'm';
  beep[1] = beep_note;
  beep[2] = beep_octave;
  beep[3] = note_delay;
  beep[4] = beep_note;
  beep[5] = beep_octave;
  beep[6] = note_delay;
  
  //play beep
  setMusic(beep, 0);
  //make sure to delay so beep is interuppted by changing beep
  //delay(*wait);
  //delay(*wait);
}

//play success beep
void success_beep(){
   char beep1[8] = "mC44C44";
   setMusic(beep1, 0);
}
  