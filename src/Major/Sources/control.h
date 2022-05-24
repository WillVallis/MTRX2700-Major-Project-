#ifndef CONTROL_HEADER
#define CONTROL_HEADER


// to access from main
// direction:
// outputMessage(sci1SerialBuffer, shopper.instructionString);

typedef struct Shopper {
    
    int referenceDistance; // serial input to set this distance
    float referenceAngle; // initialise to zero
    int measuredDistance; // taj
    float measuredAngle; // joon's

    int travelDistance;
    char longitudeDirection[5];
    int travelAngle;
    char latitudeDirection[7];

    char instructionString[50];

} Person;

void directionCalculator (Person *shopper);

void distanceAngleGeometry(Shopper *shopper);
    
#endif