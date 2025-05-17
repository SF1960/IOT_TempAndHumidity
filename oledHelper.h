
#include "globals.h"

/*******************************************************************************
*
* Set up the OLED Screen
*
*******************************************************************************/

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1                         // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D                       // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

namespace oled{

  void setup(){
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {      // Address 0x3D for 128x64
      debug_print(buffer + " SSD1306 allocation failed");
      //Serial.printf(buffer,"SSD1306 allocation failed");
      //Serial.println(buffer);
      for (;;);                                            // loop forever
    }

    display.clearDisplay();
    display.setTextSize(SMALL);
    display.setTextColor(WHITE);
    display.display();
    
  }

  // *********************************************
  // * function to print to the display
  // *********************************************
  void displayPrint(int txtSize, int x_posn, int y_posn, String txt) {
  
    display.setTextSize(txtSize);
    display.setCursor(x_posn, y_posn);
    display.print(txt);
    display.display();
  
  }

  void displayWelcome(){

    //display Welcome screen
    display.clearDisplay();
    oled::displayPrint(MEDIUM, 0, 0,  " *Welcome*");
    oled::displayPrint(MEDIUM, 0, 20, "ArduinoIOT");
    oled::displayPrint(SMALL, 0, 45, "Code by Steve Fuller");
    oled::displayPrint(SMALL, 10, 55, "....connecting....");

  }

  void showCopyright() {                      // display the copyright text on last line
    oled::displayPrint(SMALL, 35, 57, copyright);
  }
  
  void showEmail() {                          // display the email address on the last line
    oled::displayPrint(SMALL, 17, 55, email);
  }
  
  void drawHeaderLine() {                     // display a line underneath the header
    display.drawLine(5, 16, 120, 16, WHITE);
  }
  
}

