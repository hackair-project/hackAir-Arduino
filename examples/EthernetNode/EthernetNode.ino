/*
 * hackAIR Arduino Library
 * Copyright © 2016-2017 hackAir Consortium
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Ethernet node example sketch
 * @author Thanasis Georgiou
 */

#include <Ethernet.h>
#include "hackair.h"
#include "hackair_ethernet.h"

// MAC Address of the Ethernet Shield
// Nnewer shields should have a MAC address printed on a sticker
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
  Ethernet.maintain();

  // Collect data
  struct hackAirData data;
  sensor.refresh(data);

  // Send with ethernet shield
  hackAirNet.sendData(data);

  // Wait a bit (1 minute)
  delay(60000);
}
