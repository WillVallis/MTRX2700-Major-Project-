#ifndef TRACKING_H
#define TRACKING_H

// updates the array of laser values
void update_laser (unsigned long latest_sample, unsigned long laser_samples[20]);

// toggles the pan direction if object no longer tracked
void track_object (int tol, unsigned long laser_samples[20]);

// calculates average
unsigned long find_av (int num_samples, unsigned long *array);

#endif