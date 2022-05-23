#include "derivative.h"
#include "tracking.h"

#include "servo.h"

int laser_count = 0;
int delay_count = 0;
int max_count = 19;

/* 
Updates a 20-element array with the latest laser reading. Adjusts
the locations of the stored data so that the array remains at a 
maximum length of 20 elements.
*/
void update_laser (unsigned long latest_sample, unsigned long laser_samples[20]) {
  
  int i;
  
  if (laser_count < max_count) {
    
    laser_samples[laser_count] = latest_sample;
    laser_count++;
  }
  
  else {
    
    for (i = 0; i < max_count-1; i++) {
      
      laser_samples[i] = laser_samples[i+1];
    }
    
    laser_samples[max_count] = latest_sample;
  }
}      


/*
Toggles the pan direction if the LIDAR reading indicates that 
the object is no longer being tracked, to a specified tolerance.
*/
void track_object (int tol, unsigned long laser_samples[20]) {
  
  int num_samples = 10;
  unsigned long first_average;
  unsigned long second_average;
  
  // Tracking requires a full array of laser values.
  if (laser_count < max_count) {
    
    return;
  }
  
  // An 18-cycle delay to allow the LIDAR to recentre on the object. 
  if (delay_count <= max_count && delay_count != 0) {
    
    delay_count++;
  }
  
  else {
    
    delay_count = 0;
  }
  
  first_average = find_av(num_samples, &laser_samples[0]);
  second_average = find_av(num_samples, &laser_samples[num_samples]);
  
  // Switch the pan direction if there is sufficient change in the
  // measured values.
  if (second_average >= tol*first_average || second_average <= first_average/tol) {
    
    switch_dir;
    delay_count++;
  }
}

/*
Calculates and returns the average of a dataset given the 
number of samples.
*/
unsigned long find_av (int num_samples, unsigned long *array) {
  
  int i;
  unsigned long average;
  unsigned long sum = 0;
  
  for (i = 0; i < num_samples; i++) {
    
    sum += array[i];
  }
  
  average = sum/num_samples;
  return average;
}
  
      
  
  
  
  
    
       
  
  