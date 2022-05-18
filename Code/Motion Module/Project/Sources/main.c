#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "vector3.h"
#include "motion.h"

void main(void) {
  /* put your own code here */
  Vector3 rotation;
  Vector3 vector;
  Vector3 rotated_vector;
  Vector3 double_rotated_vector;
  float length;
  float tilt;

	EnableInterrupts;

  // Adjusting accelerometer readings from rotation
  rotation = vector3_create(0, 0.78539816339, 0); // This would be read from the gyro calibration
  vector = vector3_create(2, 0, 0);

  rotated_vector = vector3_rotated(vector, rotation);
  double_rotated_vector = vector3_rotated(rotated_vector, rotation);
  length = vector3_get_length(double_rotated_vector);

  motion_calibrate();
  
  for(;;) {
    _FEED_COP(); /* feeds the dog */
    tilt = get_tilt_angle();
  } /* loop forever */
  /* please make sure that you never leave main */
}
