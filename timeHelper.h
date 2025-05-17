/*******************************************************************************
*
* timeHelper.h
* Standalone helper function for connecting to ntp server
*
*******************************************************************************/

// timeHelper.h
#ifndef TIME_HELPER
#define TIME_HELPER

#include <time.h>
#include "globals.h"
#include "debugHelper.h"            // Include debug macros

namespace timeUtility{

  void obtainTime(); // Forward declaration

  void setup() {
    // Initialize time configuration
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(1000);   // Wait for NTP to sync
    obtainTime();  // Get initial time
  }
  
  // Get time from NTP server and format output string
  void obtainTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      debug_println("Failed to obtain time");
      //Serial.println("Failed to obtain time");
      return;
    }

    // Deal with GMT/BST - UK uses BST between last Sunday in March and last Sunday in October
    // Month in tm struct is 0-based (0-11)
    ntpHour = timeinfo.tm_hour;
    
    // Check if it's between March and October (BST months)
    if (timeinfo.tm_mon > 2 && timeinfo.tm_mon < 10) {
      ntpHour += 1; // Add 1 hour for BST
    }
    // Special handling for March (when BST starts)
    else if (timeinfo.tm_mon == 2) {
      // Check if we're in the last Sunday or after
      // Last Sunday is the day when (31 - day_of_week) <= date <= 31
      int lastSunday = 31 - (timeinfo.tm_wday == 0 ? 7 : timeinfo.tm_wday);
      if (timeinfo.tm_mday >= lastSunday) {
        ntpHour += 1; // Add 1 hour for BST
      }
    }
    // Special handling for October (when BST ends)
    else if (timeinfo.tm_mon == 9) {
      // Check if we're before the last Sunday
      int lastSunday = 31 - (timeinfo.tm_wday == 0 ? 7 : timeinfo.tm_wday);
      if (timeinfo.tm_mday < lastSunday) {
        ntpHour += 1; // Still in BST
      }
    }

    // Handle day wrap for hour values
    if (ntpHour > 23) {
      ntpHour -= 24;
    }

    // Add zero to hours where less than 10
    strHour = (ntpHour < 10) ? "0" + String(ntpHour) : String(ntpHour);

    // Add zero to minutes where less than 10
    strMin = (timeinfo.tm_min < 10) ? "0" + String(timeinfo.tm_min) : String(timeinfo.tm_min);

    // Add hours and minutes
    ntpTime = strHour + ":" + strMin;
  }
  
  // Return formatted date string (YYYY-MM-DD)
  String getFormattedDate() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      return "Unknown date";
    }
    
    char dateStr[11];
    sprintf(dateStr, "%04d-%02d-%02d", 
            timeinfo.tm_year + 1900, 
            timeinfo.tm_mon + 1, 
            timeinfo.tm_mday);
    return String(dateStr);
  }
  
  // Check if it's time to perform daily tasks (like resetting extremes)
  bool isDailyTaskTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      return false;
    }
    
    // For example, perform daily tasks at 00:00
    return (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0);
  }
  
}

#endif // TIME_HELPER