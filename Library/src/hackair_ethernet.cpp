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

void ethernet_sendData(const String &token, float pm25, float pm10, int battery, int tamper, int error) {
    // Open a connection
    client.connect("hackair.draxis.gr", 8000);
    
    // Send request header
    client.println("POST /sensors/arduino/measurements");
    client.println("Host: hackair.draxis.gr");
    client.println("Content-Type: application/json");
    client.print("Authorization: ");
    client.println(token);
    client.println("Connection: close");
    client.println("");
    
    // Send content
    client.print("{\"reading\":{\"PM2.5_AirPollutantValue\":\"");
    client.print(pm25);
    client.print(",\"PM10_AirPollutantValue\":\"");
    client.print(pm10);
    client.print("\"},\"battery\":\"");
    client.print(battery);
    client.print("\",\"tamper\":\"");
    client.print(tamper);
    client.print("\",\"error\":\"");
    client.print(error);
    client.prinln("\"}");
}
