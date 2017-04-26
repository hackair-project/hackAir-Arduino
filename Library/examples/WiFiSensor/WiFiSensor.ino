/**
 * @file WiFi Sensor example
 * This example reads data from a sensor (default choice is the serial SDS011)
 * and sends it to the hackAIR servers using the hackAIR WiFi Shield.
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include "hackair.h"
#include "hackair_wifi.h"

hackAIR sensor(SENSOR_SDS011);

void setup() {  
  // Initialize the sensor
  sensor.begin();

  // Boot WiFi module
  wifi_begin();
  // Wait for WiFi connection
  // At this point you should use your mobile phone to setup the WiFi connection
  wifi_waitForReady();
  // Set authentication token
  wifi_setToken("REPLACE WITH AUTHENTICATION TOKEN");  
}

void loop() {
  // Struct for storing data
  struct hackAirData data;
  sensor.refresh(data);

  // Average readings
  float pm25 = data.pm25;
  float pm10 = data.pm10;

  // We will take 60 averages
  for (int i = 0; i < 60; i++) {
    // Read from the sensor
    sensor.refresh(data);

    // If error is not zero something went wrong with this measurment
    // and we should not send it.
    if (data.error == 0) {
      // Calculate average between the new reading and the old average
      pm25 = (pm25 + data.pm25) / 2;
      pm10 = (pm10 + data.pm10) / 2;
    }

    delay(1000); // Wait one second
  }

  // Send data to the hackAIR server
  data.pm25 = pm25;
  data.pm10 = pm10;
  wifi_sendData(data);
}
