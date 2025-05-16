// environmentHelper.h
#ifndef ENVIRONMENT_HELPER_H
#define ENVIRONMENT_HELPER_H

#include <Arduino.h>
#include "globals.h"

namespace environment {
  
  // Update min/max temperature and humidity values
  void updateExtremes(float temperature, float humidity) {
    // Update min/max temperature
    if (temperature < minTemperature) minTemperature = temperature;
    if (temperature > maxTemperature) maxTemperature = temperature;
    
    // Update min/max humidity
    if (humidity < minHumidity) minHumidity = humidity;
    if (humidity > maxHumidity) maxHumidity = humidity;
    
    // Reset extremes once per day if needed
    unsigned long currentMillis = millis();
    if (currentMillis - lastExtremeReset >= EXTREMES_RESET_INTERVAL) {
      lastExtremeReset = currentMillis;
      // Don't reset to default values immediately - take current reading as new baseline
      minTemperature = maxTemperature = temperature;
      minHumidity = maxHumidity = humidity;
      Serial.println(device + "Daily reset of temperature/humidity extremes");
    }
  }
  
  // Check for significant environmental changes
  // Reports when temperature changes by 5°C or humidity by 5%
  void checkAnomalies(float temperature, float humidity) {
    // Define threshold values for significant changes
    const float TEMP_CHANGE_THRESHOLD = 5.0;   // °C
    const float HUM_CHANGE_THRESHOLD = 5.0;    // %
    
    // Static variables to store previous readings and initialization state
    static float lastTemperature = 0.0;
    static float lastHumidity = 0.0;
    static bool isInitialized = false;
    
    // Skip comparison on first run, just store initial values
    if (!isInitialized) {
      lastTemperature = temperature;
      lastHumidity = humidity;
      isInitialized = true;
      Serial.println(device + "Environmental monitoring initialized");
      return;
    }
    
    // Check for significant temperature change
    float tempDiff = abs(temperature - lastTemperature);
    if (tempDiff >= TEMP_CHANGE_THRESHOLD) {
      String message = "ALERT: Temperature changed by " + String(tempDiff, 1) + 
                      "°C (from " + String(lastTemperature, 1) + 
                      "°C to " + String(temperature, 1) + "°C)";
      
      Serial.println(device + message);
      monitor = message;
      ArduinoCloud.update();
      
      // Update stored temperature after reporting
      lastTemperature = temperature;
    }
    
    // Check for significant humidity change
    float humDiff = abs(humidity - lastHumidity);
    if (humDiff >= HUM_CHANGE_THRESHOLD) {
      String message = "ALERT: Humidity changed by " + String(humDiff, 1) + 
                      "% (from " + String(lastHumidity, 1) + 
                      "% to " + String(humidity, 1) + "%)";
      
      Serial.println(device + message);
      monitor = message;
      ArduinoCloud.update();
      
      // Update stored humidity after reporting
      lastHumidity = humidity;
    }
    
    // If no significant changes, update stored values periodically
    // This prevents small incremental changes from avoiding detection
    static unsigned long lastUpdateTime = 0;
    if (millis() - lastUpdateTime >= 3600000) { // Update reference values every hour
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastUpdateTime = millis();
    }
  }

  // Determine comfort status based on temperature and humidity
  String getComfortStatus(ComfortState comfort) {
    switch (comfort) {
      case Comfort_OK:
        return "OK";
      case Comfort_TooHot:
        return "Too Hot";
      case Comfort_TooCold:
        return "Too Cold";
      case Comfort_TooDry:
        return "Too Dry";
      case Comfort_TooHumid:
        return "Too Humid";
      case Comfort_HotAndHumid:
        return "Hot/Humid";
      case Comfort_HotAndDry:
        return "Hot & Dry";
      case Comfort_ColdAndHumid:
        return "Cold/Humid";
      case Comfort_ColdAndDry:
        return "Cold/Dry";
      default:
        return "Unknown";
    }
  }
}

#endif // ENVIRONMENT_HELPER_H