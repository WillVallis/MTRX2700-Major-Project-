#include "vector3.h"
#include "math.h"

Vector3 vector3_create(float x, float y, float z) {
    Vector3 vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;

    return vector;
}


Vector3 vector3_rotated(Vector3 vector, Vector3 rotation) {
    Vector3 rotated_vector;

    float sin_alpha = sinf(rotation.z);
    float cos_alpha = cosf(rotation.z);
    float sin_beta = sinf(rotation.y);
    float cos_beta = cosf(rotation.y);
    float sin_gamma = sinf(rotation.x);
    float cos_gamma = cosf(rotation.x);
    
    rotated_vector.x = (cos_alpha*cos_beta) * vector.x + (cos_alpha*sin_beta*sin_gamma-sin_alpha*cos_gamma) * vector.y + (cos_alpha*sin_beta*cos_gamma+sin_alpha*sin_gamma) * vector.z;
    rotated_vector.y = (sin_alpha*cos_beta) * vector.x + (sin_alpha*sin_beta*sin_gamma-cos_alpha*cos_gamma) * vector.y + (sin_alpha*sin_beta*cos_gamma+cos_alpha*sin_gamma) * vector.z;
    rotated_vector.z =             -sin_beta * vector.x +                                 cos_beta*sin_gamma * vector.y +                                 cos_beta*cos_gamma * vector.z;

    return rotated_vector;
}

Vector3 vector3_scaled_vector(Vector3 vector, Vector3 scale) {
    Vector3 scaled_vector;

    scaled_vector.x = vector.x * scale.x;
    scaled_vector.y = vector.y * scale.y;
    scaled_vector.z = vector.z * scale.z;

    return scaled_vector;
}

Vector3 vector3_offset_vector(Vector3 vector, Vector3 offset) {
    Vector3 offset_vector;

    offset_vector.x = vector.x + offset.x;
    offset_vector.y = vector.y + offset.y;
    offset_vector.z = vector.z + offset.z;

    return offset_vector;
}

Vector3 vector3_scaled_scalar(Vector3 vector, float x, float y, float z) {
    Vector3 scaled_vector;
    Vector3 scaling_vector = vector3_create(x, y, z);
    scaled_vector = vector3_scaled_vector(vector, scaling_vector);
    return scaled_vector;
}

Vector3 vector3_offset_scalar(Vector3 vector, float x, float y, float z) {
    Vector3 offset_vector;
    Vector3 offsetting_vector = vector3_create(x, y, z);
    offset_vector = vector3_scaled_vector(vector, offsetting_vector);
    return offset_vector;
}

Vector3 vector3_offset_vector(Vector3 vector, Vector3 offset) {
    Vector3 offset_vector;

    offset_vector.x = vector.x + offset.x;
    offset_vector.y = vector.y + offset.y;
    offset_vector.z = vector.z + offset.z;

    return offset_vector;
}

// Vector3 vector3_get_rotation(Vector3 vector){

// }


float vector3_get_length(Vector3 vector) {
    return sqrtf(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
}


Vector3 vector3i_to_vector3(Vector3i ivec) {
    Vector3 float_vector;

    float_vector.x = (float)ivec.x;
    float_vector.y = (float)ivec.y;
    float_vector.z = (float)ivec.z;
    
    return float_vector;
}
