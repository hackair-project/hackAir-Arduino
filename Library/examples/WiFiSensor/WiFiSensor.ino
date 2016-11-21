#include "hackair.h"
#include "hackair_wifi.h"

// Use SENSOR_DFROBOT or SENSOR_SDS011 depanding
// on your hardware.
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
  // Reading sums for averaging
  float pm25 = 0;
  float pm10 = 0;

  // We will take 60 averages
  for (int i = 0; i < 60; i++) {
    // Read from the sensor
    int error = sensor.refresh();

    // If error is not zero something went wrong with this measurment
    // and we should not send it.
    if (error == 0) {
      // Add current data to sum
      pm25 += sensor.readPM25();
      pm10 += sensor.readPM10();
    }

    delay(1000); // Wait one second
  }

  // Divide data by 60 so we get the mean value
  pm25 /= 60;
  pm10 /= 60;
  
  // Send data to the hackAIR server
  wifi_sendData(pm25, pm10, 100, 0, 0);
}
