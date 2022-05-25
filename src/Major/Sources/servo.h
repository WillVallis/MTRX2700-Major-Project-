#ifndef SERVO_H
#define SERVO_H

typedef struct Edge_vals {
  
  float dist[2];
  float angl[2];
  
} Edge_vals;

void PWMinitialise(void);

void Init_TC6 (void);

Edge_vals return_edge(void);

void switch_dir (void);

int current_dir (void);

float crnt_angle (void);

void set_rotation (int rotation_true);

// sets servo in elevation and azimuth
// note: this requires verification and calibration 
void setServoPose(int azimuth);

// interrupt used for cycling through the servo positions
__interrupt void TC6_ISR(void);


#endif