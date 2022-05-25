#include <hidef.h>      /* common defines and macros */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "pll.h"
#include "serial.h"
#include "l3g4200d.h"
#include "motion.h"
#include "math.h"
#include "music.h"
#include "ftoa.h"
#include "timer.h"
#include "sound.h"
#include "display.h"
#include "control.h"
#include "gyro.h"
#include "laser.h"
#include "servo.h"
#include "tracking.h"
#include "movement.h"
#include "misc.h"


// Define switch case constants 
const int DIRECT = 'D' + 'i' + 'r' + 'e' + 'c' + 't' + 'i' + 'o' + 'n';
const int DIST = 'D' + 'i' + 's' + 't' + 'a' + 'n' + 'c' + 'e';
const int ANG = 'A' + 'n' + 'g' + 'l' + 'e';
const int TIPPED = 'T'+'i'+'p'+'p'+'e'+'d';
const int TIME = 'T' + 'i' + 'm' + 'e';

// Tip alarm sounds 
const char TIP_ALARM[] = "mb42a42G42a42c50d52c52b42c52e50f52e52D52e52b52a52G52a52b52a52G52a52c61c61a51c61g53a53b52a51g51a51g53a53b52a51g51a51g53a53b52a51g51F51e50";

const char UNKNOWN_COMMAND_TEMPLATE[] = "Error! Unknown command.\r";
const char WELCOME_MESSAGE[] = "Hello! The device should now be tracking you! To enter a query, type one of the following:\n\
To check distance between user and system, enter \'Distance\'\n\
To check angle from datum, enter \'Angle\'\n\
To check whether the system is upright, enter \'Tipped\'\n\
To get the current directions to the user, enter \'Direction\'\r";

const char TIPPED_MESSAGE[] = "Trolley is tipped.\r";
const char UNTIPPED_MESSAGE[] = "Trolley is upright.\r";

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
   int value = 0, i = 0;
   
   // Iterate through message and add values
   for (i = 0; i < length; i++) {
     value += message[i];
   }
   
   // Return numeric message value 
   return value;
}

//conversion functions
void FloatToInt(float num, int new_num[1]){
  num += 0.5; //round to nearest whole
  new_num[0] = (int)num;
}

void MetreToCem(float metre, int cem[1]){
	metre *= 100;
	cem[0] = (int)metre;
}

// Our main loop
void main(void) {
  
  
  float tilt;
  int conv_distance, tipped = 0;
  int *distance_samp;
  unsigned long *lidar_sample;
  
  // declare shopper
  Shopper *shopper = (Shopper*)calloc(1, sizeof(Shopper));
  
  Vector3i raw;
  
 // Initialise the serial port and create a buffer object for it
  SerialBuffer *sci1SerialBuffer = sci1Init(9600);
  
  // Tracking stuff 
  Edge_vals edge_vals;
  MoveParam movement;
  unsigned long single_sample;
  float range[2];
  
  int dist_count = 0;
  int time_count = 0;
  int angl_count = 0;
  int max_count = 40;
  
  float dist_array[40];
  float angl_array[40];
  float time_array[40];
  
  float dist;
  float angl;
  float time;
  int init_delay = 0;
  int tol = 5;
  
  // set the acceptable range
  range[0] = 0;
  range[1] = 1.5;  


  // initialise PWM
  PWMinitialise();
  setServoPose(100);

  Init_TC6();
    
  /* ******************** */  

  // Initialise our modules            
  PLL_Init();
  initTimer();
  initMusic();  
  initMotion();
  laserInit();        
  _DISABLE_COP();
  motion_calibrate();

  // initialise the sensor suite
  iicSensorInit();
                                     

	EnableInterrupts;
	
	// Print welcome message
	outputMessage(sci1SerialBuffer, WELCOME_MESSAGE);
	
  for(;;) { // Infinite loop
    //start_count();

    tilt = motion_check_tipped_over();
    if (tilt) {
      setMusic(TIP_ALARM, 0);
    }   
    
    set_rotation(1);
      
    start_count();
      
    if (init_delay < 40) {
        
      init_delay++;
    }
      
    GetLatestLaserSample(&single_sample);      
    dist = lidar_to_dist(single_sample); 
    dist_count = update_array(&dist_array[0], dist, dist_count, max_count); 
      
      
    track_object(tol, &movement.dist[20], init_delay);
      
    angl = crnt_angle();
    angl_count = update_array(&angl_array[0], angl, angl_count, max_count); 


    /* ------ Section for handling user inputs ------ */
    // Check if we have a new command
    if (sci1SerialBuffer->inputReady) { 
      // User input is formatted as a single word
      char *message = sci1SerialBuffer->inputString;
      // Get integer value of message
      int messageValue = calculateMessageValue(sci1SerialBuffer->stringLength, message); 

      
      
      switch (messageValue) {
        case DIRECT: { // Direction
          dummyControlInit(shopper);
          directionCalculator(shopper);          
          outputMessage(sci1SerialBuffer, shopper->instructionString);
          break;
        }  
        case DIST: { // Distance
          outputMessage(sci1SerialBuffer, message);
          break;
        }
        case ANG: { // Angle
          tilt = get_gyro().x;
          ftoa(tilt, message, 3);
          //getRawDataGyro(&raw);
          //itostr(raw.x, message, 8);
          outputMessage(sci1SerialBuffer, message);
          break;
          
        }
        case TIPPED: { // System orientation
          tilt =  motion_check_tipped_over();
          if (tilt) {
            outputMessage(sci1SerialBuffer, TIPPED_MESSAGE);
          }
          else outputMessage(sci1SerialBuffer, UNTIPPED_MESSAGE);
          break;
          
        }
        case TIME: {
          ftoa(time, message, 3);
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
    
    // Update count 
    time = end_count();
    time_count = update_array(&time_array[0], time, time_count, max_count);
  }
}
