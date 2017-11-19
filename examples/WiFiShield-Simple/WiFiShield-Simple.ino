/**
 * @file hackAIR WiFi Shield - Simple Example
 * This example reads data from a sensor (default choice is the serial SDS011)
 * and sends it to the hackAIR servers using the hackAIR WiFi Shield every 10 minutes.
 * 
 * @author Thanasis Georgiou
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include "hackair.h"
#include "hackair_wifi.h"

hackAIR sensor(SENSOR_SDS011);

// Struct for storing data
struct hackAirData data;

void setup() {
  // Initialize the sensor
  sensor.begin();
  sensor.clearData(data);

  // Boot WiFi module
  wifi_begin();
  // Wait for WiFi connection
  // At this point you should use your mobile phone to setup the WiFi connection
  wifi_waitForReady();
  // Set the authorization token
  wifi_setToken("AUTHORIZATION TOKEN");
}

void loop() {
  sensor.clearData(data);
  sensor.refresh(data);

  // Average readings
  double pm25 = data.pm25;
  double pm10 = data.pm10;
  int error = 0;

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
    } else {
      error++;
    }

    delay(1000); // Wait one second
  }

  // Send data to the hackAIR server
  data.pm25 = pm25;
  data.pm10 = pm10;
  data.error = error;
  wifi_sendData(data);

  // Wait 10 minutes
  delay(10 * 60 * 1000UL);
}
