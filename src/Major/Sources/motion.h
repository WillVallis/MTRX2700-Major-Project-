#ifndef MOTION_HEADER
#define MOTION_HEADER

#include "vector3.h"

typedef struct SystemState {
    Vector3 gyro_offset;
    float gyro_scale;
    float accel_scale;
} SystemState;

void initMotion();

void motion_calibrate();

Vector3 get_gyro();
Vector3 get_accel();

float get_tilt_angle();
int motion_check_tipped_over();

#endif