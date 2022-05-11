#include "derivative.h"
#include "tracking.h"

#include "servo.h"


// toggles the pan direction if the LIDAR reading indicates
// that the object is no longer being tracked, to a 
// specified tolerance
void track_object (int tol, unsigned long laser_samples[20]) {
  
  int num_samples = 10;
  unsigned long first_average;
  unsigned long second_average;
  
  first_average = find_av(num_samples, &array[0]);
  second_average = find_av(num_samples, &array[num_samples]);
  
  if (second_average >= tol*first_average || second_average <= first_average/tol) {
    
    // toggle direction
  }
}

// calculates and returns the average of a dataset given
// the number of samples
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
  
      
  
  
  
  
    
       
  
  