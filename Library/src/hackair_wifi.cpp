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
 * @file Implementation of WiFi-specific code
 * @author Thanasis Georgiou
 */

#include "Arduino.h"
#include "hackair.h"
#include "hackair_wifi.h"

void wifi_begin() {
    // Begin serial communications
    Serial.begin(115200);

    // Pin modes
    pinMode(ESP_PIN_CS, OUTPUT);
    pinMode(ESP_PIN_RST, OUTPUT);

    // Power up
    wifi_powerOn();
    wifi_hardReset();

    // Wait for boot
    delay(3000);

    // Clean up serial
    while (Serial.read() != -1) {}
}

void wifi_setToken(const String &token) {
    Serial.print("e+token ");
    Serial.print(token);
    Serial.print('\n');
}

void wifi_sendData(hackAirData &data) {
    Serial.print("e+send ");
    Serial.print(data.pm25);
    Serial.print(',');
    Serial.print(data.pm10);
    Serial.print(',');
    Serial.print(data.battery);
    Serial.print(',');
    Serial.print(data.tamper);
    Serial.print(',');
    Serial.print(data.error);
    Serial.print('\n');
}

void wifi_waitForReady() {
    Serial.print("e+isready narg\n");
    while (Serial.available() > 0) {
        char in = Serial.read();
        if (in == 0x31) break;
        delay(1000);
        Serial.print("e+isready narg\n");
   }
}

void wifi_clearAPs() {
    Serial.print("e+clearap narg\n");
    delay(1000);
}

void wifi_reset() {
    Serial.print("e+restart narg\n");
    delay(1000);
}

void wifi_hardReset() {
    digitalWrite(ESP_PIN_RST, LOW);
    delay(1000);
    digitalWrite(ESP_PIN_RST, HIGH);
    delay(1000);
}

void wifi_powerOff() {
    digitalWrite(ESP_PIN_CS, LOW);
}

void wifi_powerOn() {
    digitalWrite(ESP_PIN_CS, HIGH);
}

