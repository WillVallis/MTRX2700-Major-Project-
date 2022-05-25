#include "derivative.h"
#include "misc.h"

#include <math.h>

unsigned long clock_start;
unsigned long clock_end;

int prescaler = 1;


/************************************************************
Used to start a timing a section of code.
************************************************************/

void start_count (void) {

	clock_start = TCNT;
	
	return;
}

/************************************************************
Used to finish timing a section of code. Returns the 
time to complete in s.
************************************************************/

float end_count (void) {

	float time;
	unsigned long total_count;
	
	clock_end = TCNT;
	
	if (clock_end < clock_start) {
	
		clock_end += 65536;
	}

	total_count = clock_end-clock_start;
	time = clock_to_time(total_count);
	
	return time;
}

/************************************************************
Calculates and returns the average of a dataset given the 
number of samples.
************************************************************/

float average (int num_samples, float *array) {
  
  int i;
  float average;
  float sum = 0;
  
  for (i = 0; i < num_samples; i++) {
    
    sum += array[i];
  }
  
  average = sum/num_samples;
  return average;
}

/************************************************************
Calculates and returns the sum of a dataset given the
number of samples.
************************************************************/

float sum (int num_samples, float *array) {

	int i;
	float sum = 0;

	for (i = 0; i < num_samples; i++) {

		sum += array[i];
	}

	return sum;
}

/************************************************************
Converts a clock count to the corresponding time interval
in s.
************************************************************/

float clock_to_time (unsigned long clock_cycles) {

	float clock_speed = 24000000;
	float time;
	float prescale_fact = pow(2, prescaler);

	time = (float)clock_cycles/(clock_speed/prescale_fact);
	
	return time;
}