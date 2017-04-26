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
 * @file This sketch will upgrade the firmware of the WiFi module of the
 *       WiFi shield to version 'v2'. To use, simply upload this sketch
 *       to your Arduino, connect the shield and tap the RESET button.
 *       The Green LED on the Arduino board will turn on when the
 *       upgrade is complete.
 * @author Thanasis Georgiou
 */

#include <hackair.h>
#include <hackair_wifi.h>
#include "firmware.h"

void setup() {
  // First of all, we need to detect whether the ESP is attached
  // and responding
  wifi_begin();

  // Enter raw command mode and check for any kind of response
  Serial.print("e+raw 0\n");
  Serial.print("\n");

  // If it never responds, do nothing
  if (!Serial.find('>')) {
    return;
  }

  // Delete the old firmware file
  flashFirmware();

  // Write a goodbye message
  Serial.println("-- The upgrade process should now be complete.");
  Serial.println("-- Remove power from the Arduino, remove the shield and upload another sketch");
  Serial.println("-- so that the upgrade won't run again.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
