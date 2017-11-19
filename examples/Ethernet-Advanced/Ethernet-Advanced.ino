/**
 * @file Ethernet - Advanced Example
 * This example sets up the Ethernet shield and sends data to the hackAIR
 * platform on a configurable frequency. The sensor is turned off while not
 * measuring to extend lifespan. For a simpler example check out
 * `Ethernet-Simple`.
 *
 * @author Thanasis Georgiou
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <Ethernet.h>
#include "hackair.h"
#include "hackair_ethernet.h"

// How often to measure (in minutes)
#define MEASURING_DELAY 60

// MAC Address of the Ethernet Shield
// Newer shields should have a MAC address printed on a sticker
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Initialize the Ethernet client library and the hackAir helper class
EthernetClient client;
// Write your API key here
hackAirEthernet hackAirNet(client, "AUTHORIZATION TOKEN");

// Specify your sensor
hackAIR sensor(SENSOR_SDS011);

void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  while (!Serial) {;}

  // Start the ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true) {;}
  }

  // Give the Ethernet shield a second to initialize
  delay(1000);

  // Initialize the sensor
  sensor.enablePowerControl();
  sensor.turnOn();
  sensor.begin();
}

void loop() {
  // At this point, we are either booting or after the sleep period
  // Wait for the sensor to settle
  delay(1000 * 10);

  // Check DHCP lease
  // This is required to maintain the internet connection for long periods of time
  Ethernet.maintain();

  // Collect data
  struct hackAirData data;
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

    delay(1000);  // Wait one second
  }

  // Send with ethernet shield
  data.pm25 = pm25;
  data.pm10 = pm10;
  data.error = error;
  hackAirNet.sendData(data);

  // Turn off sensor while we wait the specified time
  sensor.turnOff();
  delay(MEASURING_DELAY * 60UL * 1000UL);
  sensor.turnOn();
}
