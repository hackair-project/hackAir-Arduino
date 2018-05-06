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
* @file Implementation of hackAIR library
* @author Thanasis Georgiou
*/

#include "Arduino.h"
#include "hackair.h"

SoftwareSerial _serial(PIN_SERIAL_RX, PIN_SERIAL_TX);

/* Constructors */
hackAIR::hackAIR(int sensorType) {
    // Initialize last reading
    _sensorType = sensorType;
}

void hackAIR::begin() {
    if (_sensorType == SENSOR_DFROBOT || _sensorType == SENSOR_SDS011 || _sensorType == SENSOR_PMS5003) {
        // Serial sensors just need a software serial port
        _serial.begin(9600);
    } else if (_sensorType == SENSOR_GROVE) {
        pinMode(8, INPUT);
    }
}

void hackAIR::readData(hackAirData &data) {
    if (_sensorType == SENSOR_DFROBOT) {
        // DFRobot
        char index = 0;
        while (_serial.available() && index != 32) {
            _buff[index] = _serial.read();
            index++;
        }
        while (_serial.read() != -1) {}

        // Check the package integrity
        if (_buff[0] == 0x42 && _buff[1] == 0x4d) {
            char receiveflag = 0;
            int receiveSum = 0;

            for (int i = 0; i < 30; i++) {
                receiveSum += _buff[i];
            }

            if (receiveSum == ((_buff[30] << 8) + _buff[31])) {
                receiveSum = 0;
                receiveflag = 1;
            }

            // If package is valid
            if (receiveflag > 0) {
                // Set the error flag
                data.error = H_NO_ERROR;

                // Set data
                data.pm25 = ((_buff[8] << 8) + _buff[9]);
                data.pm10 = ((_buff[6] << 8) + _buff[7]);

                // We don't use pm0.1
                //_pm01 = ((_buff[4] << 8) + _buff[5]);

                return;
            } else {
                // Set the error flag and exit
                data.error = H_ERROR_SENSOR;
                return;
            }
        }
    } else if (_sensorType == SENSOR_SDS011) {
        // SDS011
        char index = 0;
        while (_serial.available() && index != 10) {
            _buff[index] = _serial.read();
            index++;
        }
        while (_serial.read() != -1) {}

        // Check package integrity
        if (_buff[0] == 0xAA && _buff[1] == 0xC0 && _buff[9] == 0xAB) {
            int receiveSum = 0;

            for (int i = 2; i < 8; i++) {
                receiveSum += _buff[i];
            }

            if ((receiveSum & 0xFF) == _buff[8]) {
                // Set the error flag
                data.error = H_NO_ERROR;

                // Set data
                data.pm25 = ((_buff[3] << 8) + _buff[2]) / 10.0f;
                data.pm10 = ((_buff[5] << 8) + _buff[4]) / 10.0f;

                return;
            } else {
                // Invalid package, set the error flag
                data.error = H_ERROR_SENSOR;
                return;
            }
        }
    } else if (_sensorType == SENSOR_GROVE) {
        _lastTime = millis();
        _pulseDuration = 0;

        while (millis() - _lastTime <= 2000) {
            _pulseDuration += pulseIn(8, LOW);
        }

        int ratio = _pulseDuration / 20000.0;
        data.pm10 = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // From manual
        data.error = H_NO_ERROR;
        return;
    }

    // Invalid sensor ID means something surely went wrong
    data.error = 1;
}

void hackAIR::readAverageData(hackAirData &data, uint8_t n) {
    float sum_pm25 = 0.0f;
    float sum_pm10 = 0.0f;
    uint8_t successes = 0;

    for (uint8_t i = 0; i < n; i++) {
        delay(1000); // Wait for a new reading, all sensors have a maximum
                     // 1Hz sampling rate
        readData(data);

        if (data.error == H_NO_ERROR) {
            sum_pm25 += data.pm25;
            sum_pm10 += data.pm10;

            successes += 1;
        }
    }

    data.pm25 = sum_pm25 / successes;
    data.pm10 = sum_pm10 / successes;

    if (successes != n) {
        data.error = H_ERROR_SENSOR_ONCE;
    } else if (successes == 0) {
        data.error = H_ERROR_SENSOR;
    } else {
        data.error = H_NO_ERROR;
    }
}

void hackAIR::clearData(hackAirData &data) {
    data.pm25 = 0;
    data.pm10 = 0;
    data.error = H_NO_ERROR;
    data.tamper = 0;
    data.battery = 0;
}

void hackAIR::enablePowerControl() {
#ifndef ESP8266
    pinMode(A2, OUTPUT);
#endif
}

void hackAIR::turnOn() {
    // SDS011 uses the built-in power saving function
    if (_sensorType == SENSOR_SDS011) {
        // Send anything to wake up the sensor
        _serial.write(0x01);
        delay(2000);
    } else {
#ifndef ESP8266
        digitalWrite(A2, HIGH);
#endif
    }
}

void hackAIR::turnOff() {
    // SDS011 uses the built-in power saving function
    if (_sensorType == SENSOR_SDS011) {
        // Send sleep command
        uint8_t sleep_command[] = {0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB};
        for (uint8_t i = 0; i < 19; i++) {
            _serial.write(sleep_command[i]);
        }

        // Discard response
        _serial.flush();
        while (_serial.read() != -1) {}
    } else {
#ifndef ESP8266
        digitalWrite(A2, LOW);
#endif
    }
}

// Normalization function written by Zbyszek Kiliański, Piotr Paul
// https://github.com/piotrkpaul/esp8266-sds011
void hackAIR::humidityCompensation(hackAirData &data, float humidity) {
  data.pm25 = data.pm25 / (1.0 + 0.48756 * pow((humidity / 100.0), 8.60068));
  data.pm10 = data.pm10 / (1.0 + 0.81559 * pow((humidity / 100.0), 5.83411));
}