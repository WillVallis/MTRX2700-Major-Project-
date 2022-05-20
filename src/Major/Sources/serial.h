#ifndef SERIAL_HEADER
#define SERIAL_HEADER


#define BUFFER_SIZE 450 // How many chars the buffers in the SerialBuffer should be; high values like 512 break for some reason, 450 is seemingly safe
#define TERMINATING_CHAR_INPUT '\r' // Special character used to determine the end of input
#define TERMINATING_CHAR_OUTPUT '\0' // Same for output, *really* should be \0 as that's what strings end with

// Data container for serial data i/o
typedef struct SerialBuffer {
  char portID; // Which SCI port this is for, unused for this but could theoretically be queried to check what port it's for
  char *inputBuffer; // Array buffer for input
  char *outputBuffer; // Array buffer for output

  int inputWriteIndex; // Index for where in the buffer to write to
  int outputWriteIndex; // Index for where in the buffer it is reading for during output

  int inputReady;  // Flag for if the buffer is ready to be read, read by user
  int outputReady; // Flag for if the buffer is ready to be sent, set by user

  char echoChar; // Character to echo back into the terminal
  
  char *inputString; // Copy of input buffer for safe(ish) use with commands
  int stringLength; // Length of input string 
} SerialBuffer;                  

void outputMessage(SerialBuffer *serialBuffer, char *msg);

// Support for other ports need their own init and interrupt functions as they have different control addresses and potentially different flags.

SerialBuffer *sci1Init(int baud_rate);

__interrupt void sci1Interrupt(void);

#endif
