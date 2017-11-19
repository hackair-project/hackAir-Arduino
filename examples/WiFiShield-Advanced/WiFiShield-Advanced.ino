/**
 * @file hackAIR WiFi Shield - Advanced Example
 * This example reads data from a sensor and sends it to the hackAIR platform
 * using the hackAIR WiFi Shield on a configurable frequency.
 *
 * @author Thanasis Georgiou
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <hackair.h>
#include <hackair_wifi.h>

// How often to measure (in minutes)
#define MEASURING_DELAY 60

// Define sensor
hackAIR sensor(SENSOR_DFROBOT);

// Struct for storing data
struct hackAirData data;

void setup() {
  // Initialize the sensor
  sensor.enablePowerControl();
  sensor.turnOn();
  sensor.begin();
  sensor.clearData(data);

  // Boot WiFi module
  wifi_begin();
  // Wait for WiFi connection
  // At this point you should use your mobile phone to setup the WiFi connection
  wifi_waitForReady();
  // Set authentication token
  wifi_setToken("AUTHORIZATION TOKEN");
}

void loop() {
  // At this point we are either starting up for the first time or after a
  // sleep period.
  // Wait for sensor to settle
  delay(1000 * 30);

  // Refresh data
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

    delay(1000);  // Wait one second
  }

  // Send data to the hackAIR server
  data.pm25 = pm25;
  data.pm10 = pm10;
  data.error = error;
  wifi_sendData(data);

  // Turn off sensor while we wait the specified time
  sensor.turnOff();
  delay(MEASURING_DELAY * 60 * 1000UL);
  sensor.turnOn();
}
