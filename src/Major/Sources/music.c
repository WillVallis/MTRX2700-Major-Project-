#include "music.h"
#include "timer.h"

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"

// Lowest octave C to A, lowercase by default, uppercase for sharp
// Higher octaves can be gotten from halving the values and vice versa
// Tuned for max prescaler, equation: 1000 / hz * 188
#define NOTE_C  1437
#define NOTE_CS 1356
#define NOTE_D  1281
#define NOTE_DS 1208
#define NOTE_E  1141
#define NOTE_F  1077
#define NOTE_FS 1016
#define NOTE_G  959
#define NOTE_GS 905
#define NOTE_A  855
#define NOTE_AS 807
#define NOTE_B  761
#define NOTE_N  1

const char OUTPUT_TEMPLATE[] = "Music will take __ seconds to complete.\r";
const char LOOP_TEMPLATE[] = "Music will take __ seconds to loop.\r";
const char INVALID_TEMPLATE[] = "Input format invalid!\r";
const char UNFINISHED_TEMPLATE[] = "Please wait until the song is complete.!\r";
const char STOPPED_TEMPLATE[] = "Music playback stopped.\r";

int *notes;
int *lengths;
volatile int musicLength = 0;
volatile int noteIndex = 0;

volatile int playing = 0;
volatile int looping = 0;

// Initialise the music module
void initMusic() {
    // Enable channel 6 and 5 for out two interrupts
    enableOutputCompare(6);
    enableOutputCompare(5);
    DDRT |= DDRT_DDRT5_MASK; // Enable output 5 (speaker)
    // Initialise arrays for storing the song notes and lengths
    notes = (char*)malloc(256*sizeof(char));
    lengths = (char*)malloc(256*sizeof(char));
}



// Function to play new song
//      serialBuffer    :   The serial buffer to output info to
//      command         :   The string inputed by the user, more info down
//      loop            :   Flag for if the inputed music should loop
//
// Command info:
//      The first character of the command is for determinging what command it is, this is handled by main.c. 
//      The parameters are formatted as <note><octave><duration> on loop, each being 1 character
//      The notes can be in the set [c, C, d, D, e, f, F, g, G, a, A, b]
//      Lowercase is default, uppercase is the sharp of that note, i.e. A = a#
//      Octave increases the pitch by 1 each time
//      Duration halves as the number goes up, with 0 being 350 ms
void setMusic(char *command, int loop) {
    volatile int i = 1; // Index for the input string
    int mode = 0; // Parse mode, 0: pitch, 1: octave, 2: length
    char currentCharacter = command[i++];
    int currentNoteValue;
    int timeEstimate = 0;
    char *msg;
    
    // Disable interrups so interrups don't mess up values 
    DisableInterrupts;

    // Check if a song is already playing and it's not a looping song that can be cut at anythime
    if (!playing || (playing && looping)) {
        // Set our playback flags
        playing = 1;
        looping = loop;
        
        // Iterate through each "note" in the command 
        noteIndex = 0;
        while (currentCharacter != '\0') {
            switch (mode) {
                case 0: { // Pitch parse
                    switch (currentCharacter) {
                        case 'c': 
                            currentNoteValue = NOTE_C;
                            break;
                        case 'C':
                            currentNoteValue = NOTE_CS;
                            break;
                        case 'd':
                            currentNoteValue = NOTE_D;
                            break;
                        case 'D':
                            currentNoteValue = NOTE_DS;
                            break;
                        case 'e':
                            currentNoteValue = NOTE_E;
                            break;
                        case 'f':
                            currentNoteValue = NOTE_F;
                            break;
                        case 'F':
                            currentNoteValue = NOTE_FS;
                            break;
                        case 'g':
                            currentNoteValue = NOTE_G;
                            break;
                        case 'G':
                            currentNoteValue = NOTE_GS;
                            break;
                        case 'a':
                            currentNoteValue = NOTE_A;
                            break;
                        case 'A':
                            currentNoteValue = NOTE_AS;
                            break;
                        case 'b':
                            currentNoteValue = NOTE_B;
                            break;
                        default:
                            currentNoteValue = NOTE_N;
                            break;
                    }
                    break;
                }
                case 1: { // Octave parse
                    // Right shift to halve the duration
                    if (currentNoteValue != NOTE_N) currentNoteValue = currentNoteValue >> (currentCharacter - '0'); 
                    break;
                }
                case 2: { // Duration parse
                    // Store the note data in our arrays
                    notes[noteIndex] = currentNoteValue;
                    lengths[noteIndex] = (65535) >> (currentCharacter - '0'); // 65535 = 350 ms at max prescaler
                    timeEstimate += 350 >> (currentCharacter - '0');
                    noteIndex++;
                    break;
                }
            }
            currentCharacter = command[i++];
            mode = (mode + 1) % 3;
        }
        musicLength = noteIndex;
        // Catch blank loop for end of song
        notes[noteIndex] = NOTE_N; 
        noteIndex = 0;

        // Reset the timers so the song starts immediately
        TC6 = TCNT + notes[0];
        TC5 = TCNT + lengths[0];

        // Convert to seconds, rounding up
        timeEstimate = (timeEstimate + (1000-1)) / 1000; 
    }
        EnableInterrupts;
}

// Interrupt section for this module. Placement will be in NON_BANKED area.
#pragma CODE_SEG __NEAR_SEG NON_BANKED 

// Interrupt for current note
__interrupt void noteInterrupt(void) {

    // If the current note isn't a silent note
    if (notes[noteIndex] != NOTE_N) { 
        // Flip the speaker to produce sound
        PTT ^= 0b00100000;
        // and set the next trigger to the appropriate time 
        TC6 = TCNT + notes[noteIndex]; 
    } else {
        // If it is a null note, set the next note interrupt to be at the same time as the note change
        TC6 = TC5; 
    }
}

// Interrupt for changeing notes
__interrupt void nextNoteInterrupt(void) {
    // If the song should loop and it was not stopped, loop back to the start of the song if we went pass the end
    if (looping) { 
        if (playing) noteIndex = (noteIndex + 1) % musicLength;
        else noteIndex = musicLength;
    } 
    // If the song shouldn't loop and we reached the end of the song, stop playback. Otherwise advance to the next note.
    else { 
        if (noteIndex == musicLength) playing = 0;
        else noteIndex += 1;
    }
    TC5 = TCNT + lengths[noteIndex];
}