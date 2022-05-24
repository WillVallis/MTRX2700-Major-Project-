#include "derivative.h"
#include "tracking.h"

#include <math.h>

#include "servo.h"


int laser_count = 0;
int delay_count = 0;


/* 
Updates a 20-element array with the latest laser reading. Adjusts
the locations of the stored data so that the array remains at a 
maximum length of 20 elements.
*/
void update_laser (float latest_sample, float *laser_samples) {
  
  int full_length = 20;  
  int i;
  
  if (laser_count < full_length) {
    
    laser_samples[laser_count] = latest_sample;
    laser_count++;
  }
  
  else {
    
    for (i = 0; i < full_length; i++) {
      
      laser_samples[i] = laser_samples[i+1];
    }
    
    laser_samples[full_length] = latest_sample;
  }
}      


/*
Toggles the pan direction if the LIDAR reading indicates that 
the object is no longer being tracked, to a specified tolerance.
*/
void track_object (int tol, float *laser_samples) {
  
  int full_length = 20; 
  int half_length = full_length/2;
  int delay_period = 40;
  float first_average;
  float second_average;
  
  // Tracking requires a full array of laser values.
  if (laser_count < full_length) {
    
    return;
  }
  
  // A 20-cycle delay to allow the LIDAR to recentre on the object. 
  if (delay_count <= delay_period && delay_count != 0) {
    
    delay_count++;
  }
  
  else {
    
    delay_count = 0;
    first_average = find_av(half_length, &laser_samples[0]);
    second_average = find_av(half_length, &laser_samples[half_length]);
  
    // Switch the pan direction if there is sufficient change in the
    // measured values.
    if (second_average >= tol*first_average || second_average <= first_average/tol) {
    
      switch_dir();
      delay_count++;
    }
  }
  
}

/*
Calculates and returns the average of a dataset given the 
number of samples.
*/
float find_av (int num_samples, float *array) {
  
  int i;
  float average;
  float sum = 0;
  
  for (i = 0; i < num_samples; i++) {
    
    sum += array[i];
  }
  
  average = sum/num_samples;
  return average;
}


float conv_dist(unsigned long laser_sample) {
  
  float clock_freq = 24000000;
  float conv_fact = 1000;
  
  float time;
  float dist;
  
  time = laser_sample/clock_freq;
  dist = time*conv_fact;   
  
  return dist;
}
  
  
  
      
  
  
  
  
    
       
  
  