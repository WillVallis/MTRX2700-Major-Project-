#ifndef SERVO_H
#define SERVO_H

void PWMinitialise(void);

// sets servo in elevation and azimuth
// note: this requires verification and calibration 
void setServoPose(int azimuth, int elevation);

// switches pan direction
void switch_dir (void);

// returns the current pan direction
int current_dir (void);

// interrupt used for cycling through the servo positions
__interrupt void TC6_ISR(void);

void Init_TC6 (void);


#endif