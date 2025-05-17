#pragma once

#include "globals.h"
#include "debugHelper.h"            // Include debug macros

/*******************************************************************************
*
* Set up the serial monitor with BAUD rate passed from calling function
*
*******************************************************************************/

namespace serial{

  void setup(int BAUD){

    Serial.begin(BAUD);
    debug_println(">>>> Attempting to start serial monitor at " + BAUD);
    //Serial.printf(">>>> Attempting to start serial monitor at %d\n", BAUD); 
    
    // the following code can mean that the code WILL ONLY RUN if connected to a PC
    // so ONLY use if in DEBUG mode
    if (DEBUG ==1){
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
      } 
    } else {
      delay(1000);
    }

      debug_println(">>>> Serial started at " + BAUD);
    //Serial.printf(">>>> Serial started at %d\n", BAUD);

  }

}