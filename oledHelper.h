
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
      Serial.printf(buffer,"SSD1306 allocation failed");
      Serial.println(buffer);
      for (;;);                                            // loop forever
    }

    display.clearDisplay();
    display.setTextSize(SMALL);
    display.setTextColor(WHITE);
    display.display();
    
  }

}

