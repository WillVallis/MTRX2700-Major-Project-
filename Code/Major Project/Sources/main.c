#include <hidef.h>      /* common defines and macros */
#include <stdlib.h>
#include <string.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "serial.h"


const char UNKNOWN_COMMAND_TEMPLATE[] = "Error! Unknown command: _.\r";
const char WELCOME_MESSAGE[] = "Hello! The device should now be tracking you! Feel free to enter a query!\n\
To check distance between user and system, enter \'Distance\'\n\
To check angle from datum, enter \'Angle\'\n\
To check system status, enter \'Status\'\r";

void printUnknownCommandError(SerialBuffer *serialBuffer, char command) {
  // Create a buffer for the output message
  char *msg = (char*)malloc(sizeof(UNKNOWN_COMMAND_TEMPLATE));
  // Iterate over the template and copy the text over
  int i = 0;
  while (i < sizeof(UNKNOWN_COMMAND_TEMPLATE)) {
    msg[i] = UNKNOWN_COMMAND_TEMPLATE[i];
    i++;
  }
  // Replace the '_' with the command character
  msg[24] = command;
  // Send the message to the serial buffer
  outputMessage(serialBuffer, msg);
  // Free our msg buffer
  free(msg);
}

// Our main loop
void main(void) {
  // Initialise the serial port and create a buffer object for it
  SerialBuffer *sci1SerialBuffer = sci1Init(9600);

  // Initialise our modules
  initTimer();


	EnableInterrupts;
	
	// Print welcome message
	outputMessage(sci1SerialBuffer, WELCOME_MESSAGE);
	
	
  for(;;) { // Infinite loop
    _FEED_COP(); 

    if (sci1SerialBuffer->inputReady) { // Check if we have a new command
      // User input is formatted as a single command character follwed by parameters
      char *command = sci1SerialBuffer->inputString; // First character is our command
      if (strcmp(command, "Distance\0") == 0) {
        // Case for distance query 
        // distance = getDistanceFunction();
        // outputMessage(sci1SerialBuffer, distance);
        outputMessage(sci1SerialBuffer, command);
      }
      if (strcmp(command, "Angle\0") == 0) {
        // Case for angle query
        // angle = getAngleFunction();
        // outputMessage(sci1SerialBuffer, angle);
        outputMessage(sci1SerialBuffer, command);
      }
      if (strcmp(command, "Status\0") == 0) {
        // Case for status query 
        // status = getStatusFunction();
        // outputMessage(sci1SerialBuffer, status);
        outputMessage(sci1SerialBuffer, command);
      } 
      else {
          printUnknownCommandError(sci1SerialBuffer, *command);
          break;
      }
      // We read the new message so mark as such
      sci1SerialBuffer->inputReady = 0;
    }
  }
}
