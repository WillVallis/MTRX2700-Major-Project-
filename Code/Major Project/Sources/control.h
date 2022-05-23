#ifndef control_HEADER
#define control_HEADER


// to access from main
// direction:
// outputMessage(sci1SerialBuffer, shopper.instructionString);

typedef struct Shopper {
	
    int referenceDistance; // serial input to set this distance
  	int referenceAngle;
  	int measuredDistance;
    int measuredAngle;

    int travelDistance;
    char longitudeDirection[5];
    int travelAngle;
    char latitudeDirection[7];

    char instructionString[50];

} Person;

void directionCalculator (Person *shopper);

int tof2distance (int ToF);
    
#endif