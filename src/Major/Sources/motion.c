#include "motion.h"
#include "l3g4200d.h"
#include "math.h"

SystemState systemState;

void motion_calibrate() {
    Vector3i read_vector;
    Vector3 float_vector;
    
    // Gyro calibration
    getRawDataGyro(&read_vector);
    float_vector = vector3i_to_vector3(read_vector);
    // Need to find out what the scale is
    systemState.gyro_offset = vector3_scaled_scalar(float_vector, -1.0, -1.0, -1.0);

    // Accelerometer calibration
    getRawDataAccel(&read_vector);
    float_vector = vector3i_to_vector3(read_vector);
    systemState.accel_scale = 9.81 / vector3_get_length(float_vector); // When we're standing stil only accel is gravity
}


Vector3 get_gyro() {
    Vector3i raw_gyro;
    Vector3 converted_gyro;
    float gyro_scale = systemState.gyro_scale;

    getRawDataGyro(&raw_gyro);
    converted_gyro = vector3i_to_vector3(raw_gyro);
    converted_gyro = vector3_scaled_scalar(converted_gyro, gyro_scale, gyro_scale, gyro_scale);
    converted_gyro = vector3_offset_vector(converted_gyro, systemState.gyro_offset);

    return converted_gyro;
}

Vector3 get_accel() {
    Vector3i raw_accel;
    Vector3 converted_accel;
    float accel_scale = systemState.accel_scale;

    getRawDataAccel(&raw_accel);
    converted_accel = vector3i_to_vector3(raw_accel);
    converted_accel = vector3_scaled_scalar(converted_accel, accel_scale, accel_scale, accel_scale);
    converted_accel = vector3_rotated(converted_accel, systemState.gyro_offset);

    return converted_accel;
}

float min(float x, float y) {
    if (x > y) return y;
    return x;
}

float get_tilt_angle() {
    Vector3 accel = get_accel();

    return acosf(min(accel.y / 9.81, 1.0));
}

int motion_check_tipped_over() {
    return (get_tilt_angle() > 0.75);
}
