#ifndef CONTROL_HEADER
#define CONTROL_HEADER


// to access from main
// direction:
// outputMessage(sci1SerialBuffer, shopper.instructionString);

typedef struct Shopper {
    
    float referenceDistance; // serial input to set this distance
    float referenceAngle; // initialise to zero
    float measuredDistance; // taj
    float measuredAngle; // joon's

    float travelDistance;
    char longitudeDirection[5];
    float travelAngle;
    char latitudeDirection[7];

    char instructionString[50];

} Shopper;


void distanceAngleGeometry(Shopper *shopper);

void directionCalculator (Shopper *shopper);
    
#endif