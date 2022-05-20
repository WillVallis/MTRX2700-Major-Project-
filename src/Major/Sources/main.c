#include <hidef.h>      /* common defines and macros */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "serial.h"
#include "motion.h"
#include "math.h"
#include "ftoa.h"


// Define switch case constants 
const int DIST = 'D' + 'i' + 's' + 't' + 'a' + 'n' + 'c' + 'e';
const int ANG = 'A' + 'n' + 'g' + 'l' + 'e';
const int STAT = 'S' + 't' + 'a' + 't' + 'u' + 's';


const char UNKNOWN_COMMAND_TEMPLATE[] = "Error! Unknown command.\r";
const char WELCOME_MESSAGE[] = "Hello! The device should now be tracking you! Feel free to enter a query!\n\
To check distance between user and system, enter \'D\'\n\
To check angle from datum, enter \'A\'\n\
To check system status, enter \'S\'\r";

void printUnknownCommandError(SerialBuffer *serialBuffer) {
  // Create a buffer for the output message
  char *msg = (char*)malloc(sizeof(UNKNOWN_COMMAND_TEMPLATE));
  // Iterate over the template and copy the text over
  int i = 0;
  while (i < sizeof(UNKNOWN_COMMAND_TEMPLATE)) {
    msg[i] = UNKNOWN_COMMAND_TEMPLATE[i];
    i++;
  }

  // Send the message to the serial buffer
  outputMessage(serialBuffer, msg);
  // Free our msg buffer
  free(msg);
}

int calculateMessageValue (int length, char *message) {
   int value = 0, counter = 0;
   
   // Iterate through message and add values
   for (counter = 0; counter < length; counter++) {
     value += message[counter];
   }
   
   // Return numeric message value 
   return value;
}

// Our main loop
void main(void) {
  // float tilt;

  // Initialise the serial port and create a buffer object for it
  SerialBuffer *sci1SerialBuffer = sci1Init(9600);

  // Initialise our modules
  initTimer();
  


	EnableInterrupts;
	
	// Print welcome message
	outputMessage(sci1SerialBuffer, WELCOME_MESSAGE);
	
	//motion_calibrate();
	
  for(;;) { // Infinite loop
    _FEED_COP(); 

    if (sci1SerialBuffer->inputReady) { // Check if we have a new command
      // User input is formatted as a single command character follwed by parameters
      char command = sci1SerialBuffer->inputString[0]; // First character is our command
      char *message = sci1SerialBuffer->inputString; // Some functions need parameters
      int messageValue = calculateMessageValue(sci1SerialBuffer->stringLength, message); // Get integer value of message

      
      
      switch (messageValue) {
        case DIST: { // Distance
          outputMessage(sci1SerialBuffer, message);
          break;
        }
        case ANG: { // Angle
          //tilt = get_tilt_angle();
          //ftoa(tilt, message, 4);
          outputMessage(sci1SerialBuffer, message);
          break;
        }
        case STAT : { // Status 
          outputMessage(sci1SerialBuffer, message);
          break;
        }
        // Default cases for no command or an unknown command
        case '\0': {
          break;
        }

        default: {
          printUnknownCommandError(sci1SerialBuffer);
          break;
        }
      }
      // We read the new message so mark as such
      sci1SerialBuffer->inputReady = 0;
    }
  }
}
