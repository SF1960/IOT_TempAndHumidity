/*******************************************************************************
*
* systemHelper.h
* Standalone function for system health reporting
*
*******************************************************************************/

// systemHealthHelper.h
#ifndef SYSTEM_HEALTH_HELPER_H
#define SYSTEM_HEALTH_HELPER_H

#include <Arduino.h>
#include "globals.h"

namespace systemHealth {

  // Generate a comprehensive health report
  String generateHealthReport() {
    // Calculate uptime in hours and minutes
    String uptime = String((millis() - startTime) / 3600000) + "h " + 
                   String(((millis() - startTime) % 3600000) / 60000) + "m";
    
    // Get system information - improved temperature reading
    float cpuTemp;
    #ifdef CONFIG_IDF_TARGET_ESP32C3
      // ESP32-C3 specific method
      cpuTemp = (float)temperatureRead();
      // The raw reading needs calibration - typically offset by 10-20°C
      cpuTemp = cpuTemp - 20.0; // Adjust this offset based on testing
    #else
      // Standard method with Fahrenheit conversion
      cpuTemp = (temperatureRead() - 32) / 1.8;
    #endif
    
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t heapSize = ESP.getHeapSize();
    uint8_t heapUsedPercent = 100 - ((freeHeap * 100) / heapSize);
  
    // WiFi quality metrics
    int rssi = WiFi.RSSI();
    String wifiQuality;
    if (rssi > -50) wifiQuality = "Excellent";
    else if (rssi > -60) wifiQuality = "Very Good";
    else if (rssi > -70) wifiQuality = "Good";
    else if (rssi > -80) wifiQuality = "Fair";
    else wifiQuality = "Poor";
  
    String wifiStats = String(rssi) + "dBm (" + wifiQuality + ")";
    if (wifiDisconnections > 0) {
      wifiStats += " | Disconnects: " + String(wifiDisconnections);
    }
    
    // Generate the basic report string
    String report = "HEALTH | Uptime: " + uptime + 
                   " | CPU: " + String(cpuTemp, 1) + "°C" +
                   " | Heap: " + String(freeHeap/1024) + "K (" + String(heapUsedPercent) + "% used)" + 
                   " | WiFi: " + wifiStats +
                   " | Syncs: " + String(successfulSyncs) +
                   " | Flash: " + String(ESP.getFlashChipSize()/1024/1024) + "MB";
    
    // Add environmental extremes
    report += " | EXTREMES: Temp " + String(minTemperature, 1) + "°C to " + 
              String(maxTemperature, 1) + "°C | Hum " + 
              String(minHumidity, 1) + "% to " + String(maxHumidity, 1) + "%";
   
    return report;
  }
  
  // System report displayed on USBMonitor every hour
  void reportSystemHealth() {
    if (millis() - lastSystemReport >= SYSTEM_REPORT_INTERVAL) {
      lastSystemReport = millis();
      
      // Get the health report
      String healthReport = "HOURLY report | ";
      healthReport += generateHealthReport();
      
      // Add transfer number to the report (specific to periodic reporting)
      healthReport += " | Transfers: " + String(transferNumber);
      
      // Log to Serial
      Serial.println("=== HOURLY SYSTEM HEALTH REPORT ===");
      Serial.println(healthReport);
      Serial.println("===================================");
      
      // Send to cloud
      monitor = healthReport;
      ArduinoCloud.update();
    }
  }
  
  // Restart after 24 hours
  void checkUptimeRestart() {
    unsigned long currentTime = millis();
  
    // 1 minute warning before restart
    if (currentTime - startTime >= RESTART_INTERVAL - 60000 && 
        currentTime - startTime < RESTART_INTERVAL - 59000) {
      monitor = "WARNING: Planned restart in 1 minute";
      ArduinoCloud.update();
    }
    
    // Check if 24 hours have passed
    if (currentTime - startTime >= RESTART_INTERVAL) {
      // Log restart to monitor variable
      monitor = "Planned restart after 24h uptime";
      ArduinoCloud.update();
      
      // Small delay to ensure the message is sent
      delay(1000);
      
      // Restart the device
      Serial.println("Performing planned 24h restart...");
      Serial.flush();
      ESP.restart();
    }
  }
}

#endif // SYSTEM_HEALTH_HELPER_H