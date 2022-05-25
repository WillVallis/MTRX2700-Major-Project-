#ifndef MOVEMENT_H
#define MOVEMENT_H

typedef struct MoveParam {
  
  float dist[40];
  float time[40];
  float angl[40];
  
} MoveParam;

MoveParam update_movement (MoveParam array, float value, char type);

int update_array (float *array, float value, int current_element, int max_length);

float lidar_to_dist(unsigned long laser_sample);

float movement_speed (float *dist, float *time);

float distanceAngleGeometry (float leftAngle, float rightAngle, float leftDistance, float rightDistance);

#endif