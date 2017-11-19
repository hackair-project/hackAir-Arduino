/**
 * @file Ethernet - Simple Example
 * This example sets up the Ethernet shield and sends data to the hackAIR
 * platform every one minute.
 * 
 * @author Thanasis Georgiou
 * 
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <Ethernet.h>
#include "hackair.h"
#include "hackair_ethernet.h"

// MAC Address of the Ethernet Shield
// Shields have a MAC address printed on a sticker
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the Ethernet client library and the hackAir helper class
EthernetClient client;
hackAirEthernet hackAirNet(client, "AUTHORIZATION TOKEN"); // Write your API key here

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
  sensor.begin();
}

void loop() {
  // Check DHCP lease
  // This is required to maintain the internet connection for long periods of time
  Ethernet.maintain();

  // Collect data
  struct hackAirData data;
  sensor.clearData(data);
  sensor.refresh(data);

  // Send with ethernet shield
  hackAirNet.sendData(data);

  // Wait a bit (1 minute)
  delay(60000);
}
