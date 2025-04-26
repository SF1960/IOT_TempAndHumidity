#pragma once

/**************************************************************************************
* USER DEFINED PROGRAM INPUTS
*
* Please change your preferences below 
*
***************************************************************************************/

// definitions

#define SCREEN_WIDTH 128                          // OLED display width, in pixels
#define SCREEN_HEIGHT 64                          // OLED display height, in pixels
#define SMALL 1                                   // OLED library text sizes as an integer 
#define MEDIUM 2
#define LARGE 3
#define dhtPin A2                                 // pin for DHT
//#define BAUD 115200                               // serial monitor BAUD rate - this is now set in setup()
#define IOT_CONNECT_TIME 30000                    // wait time in ms for IOT connection

#define DEBUG 0                                   // after testing change value to 0. When set to 1 the device will need to be connected to a PC !

// Delay Constants;
int const short_time = 2000;                      // 2 second delay
int const long_delay = 4000;                      // 4 sceond delay

// IOT callback variables
bool flagSync = false;                            // IoT data are synced ?
bool flagConnect = false;                         // IOT Callback connected ?
bool flagDisconnect = true;                       // IOT initialise
long int startDisconnect;                         // start disconnection timer

// Author and devices constants
String const copyright = "(C)SGF2023/4";          // string to hold copyright information
String const email = "sgfpcb@gmail.com";          // string to hold email address
String const device = "#23: ";                    // board number for Serial Print command
String const newLine = "\n";

// graph variables
bool Redraw1 = true;                              
bool Redraw3 = true;

// temperature and humidity variables
float lowTempVal = 0;                             // low scale temperature
float highTempVal = 40;                           // high scale temperature
float lowHumVal = 0;                              // low humidity scale
float highHumVal = 100;                           // high humidity scale

// time variables
const char* ntpServer = "pool.ntp.org";           // ntp server
const long  gmtOffset_sec = 0;                    // gmt offset
const int   daylightOffset_sec = 0;               // daylight saving
String ntpTime ="00:00";                          // time variable
String strHour = "0";                             // hour variable
String strMin = "0";                              // minute variable
int ntpHour = 0;                                  // integer value for the hour
int ntpMin = 0;                                   // integer value for the minutes
unsigned long  transferNumber = 0;                // the number of transfers to IOT

// restart ESP variables
const unsigned long RESTART_INTERVAL = 86400000;  // 24 hours in milliseconds
unsigned long startTime;

char buffer[40];
