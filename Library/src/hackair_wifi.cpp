/**
 * hackAIR Arduino Library
 * @author Thanasis Georgiou
 * A library for the Arduino platform for using a variety
 * of air quality and dust sensors with the aim of croudsourcing
 * data.
 *
 * (C) hackAIR Consortium, 2016
 * This deliverable contains original unpublished work except
 * where clearly indicated otherwise. Acknowledgement of previously
 * published material and of the work of others has been made through
 * appropriate citation, quotation or both.
 * Reproduction is authorised provided the source is acknowledged.
 */

#include "Arduino.h"
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

void wifi_sendData(float pm25, float pm10, int battery, int tamper, int error) {
    Serial.print("e+send ");
    Serial.print(pm25);
    Serial.print(',');
    Serial.print(pm10);
    Serial.print(',');
    Serial.print(battery);
    Serial.print(',');
    Serial.print(tamper);
    Serial.print(',');
    Serial.print(error);
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

