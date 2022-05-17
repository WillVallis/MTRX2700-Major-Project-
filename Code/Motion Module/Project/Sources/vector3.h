#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER

typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 vector3_create(float x, float y, float z);

Vector3 vector3_rotated(Vector3 vector, Vector3 rotation);
Vector3 vector3_scaled_vector(Vector3 vector, Vector3 scale);
Vector3 vector3_offset_vector(Vector3 vector, Vector3 offset);
Vector3 vector3_scaled_scalar(Vector3 vector, float x, float y, float z);
Vector3 vector3_offset_scalar(Vector3 vector, float x, float y, float z);

// Vector3 vector3_get_rotation(Vector3 vector);
float vector3_get_length(Vector3 vector);

// Integer version of Vector3
typedef struct Vector3i
{
    int x;
    int y;
    int z;
} Vector3i;

Vector3 vector3i_to_vector3(Vector3i ivec);

#endif
