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
    
    // Mark the buffer to be ready to send an output
    serialBuffer->outputReady = 1;
    // Enable interrupts for writing to the serial port 
    SCI1CR2 |= SCI1CR2_SCTIE_MASK; 
}

// ------------------------------
// |  SCI1 specific functions   |
// ------------------------------

// Global variable because of interrupts not taking in arguments :(
SerialBuffer *sci1SerialBuffer; 

// Function for initialising the SCI1
SerialBuffer *sci1Init(int baud_rate) {
    // Clear setting bits for SCI1 by setting it all to 0
    SCI1CR1 = 0;
    // Enable read/write, and enable interrupts for new character 
    SCI1CR2 = SCI1CR2_RE_MASK | SCI1CR2_TE_MASK | SCI1CR2_RIE_MASK;
    // Set baud data register 
    SCI1BD = (24000000 / 16 / baud_rate);  

    // Create a SerialBuffer object to keep track of data
    sci1SerialBuffer = (SerialBuffer*)calloc(1, sizeof(SerialBuffer)); 
    
    // Set buffer object variables
    sci1SerialBuffer->portID = 1;
    
    // Allocate memory for our three buffers
    sci1SerialBuffer->inputBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sci1SerialBuffer->outputBuffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    sci1SerialBuffer->inputString = (char*)calloc(BUFFER_SIZE, sizeof(char));

    return sci1SerialBuffer;
}

// Interrupt section for this module. Placement will be in NON_BANKED area.
#pragma CODE_SEG __NEAR_SEG NON_BANKED 

// Interrupt function for SCI1
__interrupt void sci1Interrupt(void) {
    char status = SCI1SR1;

    // Prioritise input as to not lose user inputs
    // Receive data is available
    if (status & SCI1SR1_RDRF_MASK) { 
        char newChar = SCI1DRL;
        // Store new character into input buffer
        sci1SerialBuffer->inputBuffer[sci1SerialBuffer->inputWriteIndex++] = newChar;
        
        // Copy the inputed character over to the echoChar variable to echo back out  
        sci1SerialBuffer->echoChar = newChar; 
        // Enable interrupts for writing to the serial port to echo character
        SCI1CR2 |= SCI1CR2_SCTIE_MASK; 
        
        // If the character recieved is our terminating character, it's the end of a command
        if (newChar == TERMINATING_CHAR_INPUT) {
            // Copy over the string in the input buffer into a safer input string buffer
            int i = 0;
            // Iterate over except for the newline
            while (i < sci1SerialBuffer->inputWriteIndex-1) { 
                sci1SerialBuffer->inputString[i] = sci1SerialBuffer->inputBuffer[i];
                i++;
            }
            
            // Mark end of string
            sci1SerialBuffer->inputString[i] = '\0';
            // Reset index for the input buffer for the next time we read 
            sci1SerialBuffer->inputWriteIndex = 0; 
            // Mark the input buffer to be ready to be read
            sci1SerialBuffer->inputReady = 1;
            sci1SerialBuffer->stringLength = i;
        }
    }
    // Data is transmittable
    else if (status & SCI1SR1_TDRE_MASK) { 
        // Prioritise printing message before input echo, this won't make the echo disappear, just delayed until the message is finished printing

        // We have data in the output buffer
        if (sci1SerialBuffer->outputReady) { 
            // Get the next character to output to the serial port
            char outChar = sci1SerialBuffer->outputBuffer[sci1SerialBuffer->outputWriteIndex++]; 
             
             // If the current character is our terminating character, we should stop
            if (outChar == TERMINATING_CHAR_OUTPUT) {
                sci1SerialBuffer->outputReady = 0;
            
                // Reset the write index for next write
                sci1SerialBuffer->outputWriteIndex = 0; 
                
                // Disable interrups for writing to serial port because we are at the end of the string
                SCI1CR2 &= ~SCI1CR2_SCTIE_MASK; 
            } else {
                SCI1DRL = outChar; // Send the character to the serial port
            }
        }
        // A character is available to echo back
        else if (sci1SerialBuffer->echoChar) {
            // Send the character to the serial port 
            SCI1DRL = sci1SerialBuffer->echoChar;
            // Clear the echo character variable 
            sci1SerialBuffer->echoChar = 0;
            // Disable interrups for writing to serial port 
            SCI1CR2 &= ~SCI1CR2_SCTIE_MASK;  
        }
    }
    
}