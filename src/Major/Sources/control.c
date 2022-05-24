#include <stdio.h>
#include <string.h>
#include <math.h>

#include "control.h"
#include "ftoa.h"


// input is right and left angle
// uses geometry to find the angle of lidar sensor and the distance of the shopper
void distanceAngleGeometry (Shopper *shopper) {
    
    // considering origin is taken from the left
    float midpointAngle;
    float midpointDistance; // left = a right = b
    
    // dummy values
    float rightAngle = 5.00;
    float leftAngle = 4.00;
    float rightDistance = 0.5;
    float leftDistance = 1; 
    
    // values for calculation
    float DOC;
    float CD;
    float CM;
    float DCO;
    float COM;
    
    
    // geometry
    DOC = rightAngle - leftAngle;
    CD = sqrtf(powf(leftDistance,2) + powf(rightDistance,2)-2*leftDistance*rightDistance*cosf(DOC));
    CM = CD/2;
    DCO = asinf((rightDistance*sinf(DOC))/CD);
    midpointDistance = sqrtf(powf(leftDistance,2)+powf(CM,2) - 2*leftDistance*CM*cosf(DCO));

    COM = asinf((CM*sinf(DCO))/midpointDistance);
    midpointAngle = COM + leftAngle;

    // update shopper struct
    shopper->measuredDistance = midpointDistance;
    shopper->measuredAngle = midpointAngle;
}
     

// DIRECTION function
// INPUT: lidar value and current angle of lidar sensor
// OUTPUT: directions for wheel module
// direction [angle,longitudinal distance to person]
    
void directionCalculator (Shopper *shopper) {
    char append[64];      
 // ANGLE
    shopper->travelAngle = shopper->measuredAngle - shopper->referenceAngle;

    // once angle direction is stored, set calibrated angle as the measured angle

    // left or right
    if (shopper->travelAngle > 0) {
        strcpy(shopper->latitudeDirection, "RIGHT");
    }
    else if (shopper->travelAngle < 0) {
        strcpy(shopper->latitudeDirection, "LEFT");
    }
    else {
        strcpy(shopper->latitudeDirection, "STILL");
    }

// DISTANCE
    // cal = 1m; shopper moves 0.5m away
    shopper->travelDistance = shopper->measuredDistance - shopper->referenceDistance;

    // forward or back
    if (shopper->travelDistance > 0) {
        strcpy(shopper->longitudeDirection, "FORWARD");
    }
    else if (shopper->travelDistance < 0) {
        strcpy(shopper->longitudeDirection, "BACK");
    }
    else {
        strcpy(shopper->longitudeDirection, "STILL");
    }

// store in instruction string - to be outputted by serial module
    // want positive values for angle and distance
    shopper->travelAngle = fabs(shopper->travelAngle);
    shopper->travelDistance = fabs(shopper->travelDistance);


    // latitude (angle)
    strcat(shopper->instructionString,shopper->latitudeDirection); // add right/left
    strcat(shopper->instructionString," "); // add space
    ftoa(shopper->travelAngle, append, 2);
    strcat(shopper->instructionString,append);// add angle
    strcat(shopper->instructionString,"\n"); // new line
    
    // longitude (distance)
    strcat(shopper->instructionString,shopper->longitudeDirection); // forward/back
    strcat(shopper->instructionString," "); // add space
    ftoa(shopper->travelDistance, append, 2);
    strcat(shopper->instructionString,append); // add distance
    strcat(shopper->instructionString,"\r"); // carriage return
}
