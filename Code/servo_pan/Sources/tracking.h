#ifndef TRACKING_H
#define TRACKING_H

// toggles the pan direction if object no longer tracked
void track_object (int tol, unsigned long laser_samples[20]);

// calculates average
unsigned long find_av (int num_samples, unsigned long *array);

#endif