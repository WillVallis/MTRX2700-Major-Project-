#include "ftoa.h"
#include <math.h>
#include <stdio.h>

// Code borrowed from https://www.geeksforgeeks.org/convert-floating-point-number-string/
// as the default ftoa was not working

// Reverses a string 'str' of length 'len'
void reverse(char *str, int len) {
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
  
// Converts a given integer x to string *str. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int itostr(int x, char *str, int d) {
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}
  
// Converts a floating-point/double number to a string.
void ftoa(float n, char *str, int decPoints) {
    // Extract integer part
    int intPart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)intPart;
  
    // convert integer part to string
    int i = itostr(intPart, str, 0);
  
    // check for display option after point
    if (decPoints != 0) {
        str[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, decPoints);
  
        itostr((int)fpart, str + i + 1, decPoints);
    }
}