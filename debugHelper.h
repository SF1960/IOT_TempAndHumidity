/*******************************************************************************
*
* debugHelper.h
* Standalone function for replacing Serial.Print statements with debug_print macro
* This doesn't compile the Serial.Print statements if DEBUG = 0
*
*******************************************************************************/

#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

// Global DEBUG flag - change this in one place to enable/disable all debug output
#ifndef DEBUG
#define DEBUG 1  // Set to 0 to disable all debug prints
#endif

// Debug print macros
#if DEBUG == 1
  #define debug_print(x)   Serial.print(x)
  #define debug_println(x) Serial.println(x)
#else
  #define debug_print(x)
  #define debug_println(x)
#endif

#endif // DEBUG_HELPER_H
