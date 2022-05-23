# MTRX2700-Major-Project-
Repo for MTRX2700 Major Project Semester 1 2022

## Visual Module
  The purpose of the visual module is to display the distance between the tracked person and the trolley on the 4 7-seg displays. 

  ### Function
    -	It takes an integer value, ideally the 4-digit measured distance from the control module
    -	The module then converts this integer into an array of digits
    -	This digit array is then converted into a character array with the corresponding 7-seg values
    -	Finally, this character array is sent to the 7-seg ports
  ### Usage
    -	This module can be called either with either the ‘display_distance(int)’ function and an int, and be used in a loop 
        to constantly display the distance 
    -	Or the function ‘display_error()’ can be called with no input to display an error message on the 7-segs

## Sound Module
  The purpose of the sound module is to display a beep that increases in frequency as the tracked person gets closer to the trolley once they cross the ‘threshold’ 
  distance. The threshold distance is a user defined value to measure when the tracked person is too close, e.g. “threshold = 100;”
  
  ### Function
    -	This function utilises the music module from the previous assignment 
    -	This module takes the threshold distance, and the measured distance as integers and compares them
    -	Based on the ‘closeness’ score, a variable delay length is saved
    -	A predefined octave, note, and the delay variable are entered into a character array
    -	This character array is then used in the function call for the ‘set_music’ function from the music module 
    -	A delay is then used to wait for the beep to play

  ### Usage
    -	This function would be called after an if statement that checks if the measured distance is less than or equal to the  
        threshold distance
    -	If it is the ‘caution_beep’ function is called with the two values
    -	The two beeps in the music array play with the corresponding delay between notes
    -	The distance is measured again and the function is or isn’t called again
      - If it is called the delay value changes based on the difference in measured distance, and threshold distance, therefore  
          beeping faster if the tracked person gets closer, or slower if they get further

## Control Module
  The aim of this module is to use the information stored in the shopper struct and make calculations to obtain directions to the shopper and convert the time of flight to distance(cm).
  
  ## Function
  The control module has two functions.
  1. directionCalculator
  
  This calculates the direction the trolley needs to follow in order to reach the shopper. It provides 
 
 
  2. tof2distance
  
  This function calculates the distance from the Lidar sensor reading. It uses the scale that 1ms = 1m to calculate the distance. Distance is returned as centimetres as it is more precise for the project's application. It essentially returns an instruction string detailing the direction, which will be printed to the terminal, an example being:
  
    right 5rad

    forward 50cm
