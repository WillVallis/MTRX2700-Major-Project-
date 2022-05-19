#include "serial.h"

#include "derivative.h"

// ------------------------------
// |  General serial functions  |
// ------------------------------

// Queue up an output message into a SerialBuffer
void outputMessage(SerialBuffer *serialBuffer, char *msg) {
    // Iterate over the message string and copy over the characters into the output buffer
    int i = 0;
    char currentChar = 1;
    while (currentChar != '\0') {
        currentChar = msg[i];
        serialBuffer->outputBuffer[i] = currentChar;
        i++;
    }
    
    serialBuffer->outputReady = 1; // Mark the buffer to be ready to send an output
    SCI1CR2 |= SCI1CR2_SCTIE_MASK; // Enable interrupts for writing to the serial port
}

// ------------------------------
// |  SCI1 specific functions   |
// ------------------------------

SerialBuffer *sci1SerialBuffer; // Global variable because of interrupts not taking in arguments :(

// Function for initialising the SCI1
SerialBuffer *sci1Init(int baud_rate) {
    SCI1CR1 = 0; // Clear setting bits for SCI1 by setting it all to 0
    SCI1CR2 = SCI1CR2_RE_MASK | SCI1CR2_TE_MASK | SCI1CR2_RIE_MASK; // Enable read/write, and enable interrupts for new character
    SCI1BD = (24000000 / 16 / baud_rate); // Set baud data register 

    sci1SerialBuffer = (SerialBuffer*)calloc(1, sizeof(SerialBuffer)); // Create a SerialBuffer object to keep track of data
    
    // Set buffer object variables
    sci1SerialBuffer->portID = 1;
    // Allocate memory for our three buffers
    sci1SerialBuffer->inputBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sci1SerialBuffer->outputBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sci1SerialBuffer->inputString = (char*)calloc(BUFFER_SIZE, sizeof(char));

    return sci1SerialBuffer;
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED // Interrupt section for this module. Placement will be in NON_BANKED area.

// Interrupt function for SCI1
__interrupt void sci1Interrupt(void) {
    char status = SCI1SR1;

    // Prioritise input as to not lose user inputs

    if (status & SCI1SR1_RDRF_MASK) { // Receive data is available
        char newChar = SCI1DRL;
        sci1SerialBuffer->inputBuffer[sci1SerialBuffer->inputWriteIndex++] = newChar; // Store new character into input buffer
        sci1SerialBuffer->echoChar = newChar; // Copy the inputed character over to the echoChar variable to echo back out 
        SCI1CR2 |= SCI1CR2_SCTIE_MASK; // Enable interrupts for writing to the serial port to echo character
        // If the character recieved is our terminating character, it's the end of a command
        if (newChar == TERMINATING_CHAR_INPUT) {
            // Copy over the string in the input buffer into a safer input string buffer
            int i = 0;
            while (i < sci1SerialBuffer->inputWriteIndex-1) { // Iterate over except for the newline
                sci1SerialBuffer->inputString[i] = sci1SerialBuffer->inputBuffer[i];
                i++;
            }
            sci1SerialBuffer->inputString[i] = '\0'; // Mark end of string
            sci1SerialBuffer->inputWriteIndex = 0; // Reset index for the input buffer for the next time we read
            // Mark the input buffer to be ready to be read
            sci1SerialBuffer->inputReady = 1;
        }
    }
    else if (status & SCI1SR1_TDRE_MASK) { // Data is transmittable
        // Prioritise printing message before input echo, this won't make the echo disappear, just delayed until the message is finished printing

        if (sci1SerialBuffer->outputReady) { // We have data in the output buffer
            char outChar = sci1SerialBuffer->outputBuffer[sci1SerialBuffer->outputWriteIndex++]; // Get the next character to output to the serial port
             // If the current character is our terminating character, we should stop
            if (outChar == TERMINATING_CHAR_OUTPUT) {
                sci1SerialBuffer->outputReady = 0; 
                sci1SerialBuffer->outputWriteIndex = 0; // Reset the write index for next write
                SCI1CR2 &= ~SCI1CR2_SCTIE_MASK; // Disable interrups for writing to serial port because we are at the end of the string
            } else {
                SCI1DRL = outChar; // Send the character to the serial port
            }
        }
        else if (sci1SerialBuffer->echoChar) { // A character is available to echo back
            SCI1DRL = sci1SerialBuffer->echoChar; // Send the character to the serial port
            sci1SerialBuffer->echoChar = 0; // Clear the echo character variable
            SCI1CR2 &= ~SCI1CR2_SCTIE_MASK; // Disable interrups for writing to serial port 
        }
    }
    
}