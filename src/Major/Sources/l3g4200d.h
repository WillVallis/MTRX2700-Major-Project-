#ifndef L3G4200D_HEADER
#define L3G4200D_HEADER

// file l3g4200d.h
//   public interface for the l3g4200d sensor suite
//   this interface allows you to initalise the sensors
//   and read the raw data values
//   
//   these values require further scaling according to the 
//   datasheets

#include "iic.h"
#include "vector3.h"

// Initialise each sensor
IIC_ERRORS iicSensorInit(void);

// Get the raw acceleration data from the sensor
IIC_ERRORS getRawDataAccel(Vector3i *raw_data);

// Get the raw gyro data from the sensor
IIC_ERRORS getRawDataGyro(Vector3i *raw_data);

#endif