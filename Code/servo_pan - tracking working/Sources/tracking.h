#ifndef TRACKING_H
#define TRACKING_H

// updates the array of laser values
void update_laser (float latest_sample, float *laser_samples);


// toggles the pan direction if object no longer tracked
void track_object (int tol, float *laser_samples);

// calculates average
float find_av (int num_samples, float *array);

float conv_dist(unsigned long laser_sample);


#endif