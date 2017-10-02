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
 * @file Implementation of Ethernet-specific code
 * @author Thanasis Georgiou
 */

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "hackair_ethernet.h"

hackAirEthernet::hackAirEthernet(EthernetClient ethernet, char *token) {
  _ethernet = ethernet;
  _token = token;
}

int hackAirEthernet::sendData(hackAirData &data) {
    // Convert data to string
    String dataJson = "{\"reading\":{\"PM2.5_AirPollutantValue\":\"";
    dataJson += data.pm25;
    dataJson += "\",\"PM10_AirPollutantValue\":\"";
    dataJson += data.pm10;
    dataJson += "\"},\"battery\":\"";
    dataJson += data.battery;
    dataJson += "\",\"tamper\":\"";
    dataJson += data.tamper;
    dataJson += "\",\"error\":\"";
    dataJson += data.error;
    dataJson += "\"}";

    // Open a connection
    if (!_ethernet.connect("api.hackair.eu", 80)) {
        return 1;
    }

    // Send request headers
    _ethernet.println("POST /sensors/arduino/measurements HTTP/1.1");

    _ethernet.println("Host: api.hackair.eu");
    _ethernet.println("Content-Type: application/json");
    _ethernet.println("Accept: application/vnd.hackair.v1+json");
    _ethernet.println("Connection: close");

    _ethernet.print("Authorization: ");
    _ethernet.println(_token);

    _ethernet.print("Content-Length: ");
    _ethernet.println(dataJson.length() + 2);

    _ethernet.println("");

    // Send content
    _ethernet.println(dataJson);

    // Disconnect
    delay(500);
    while (_ethernet.available()) {
        _ethernet.read();
    }
    _ethernet.stop();

    return 0;
}
