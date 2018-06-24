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
* @file Header file for the hackAIR library
* @author Thanasis Georgiou
*/

#ifndef hackAIR_h
#define hackAIR_h

// Library version
#define HACKAIR_VERSION "0.6.1"

// Sensors
#define SENSOR_DFROBOT 1
#define SENSOR_PMS5003 1 // Rebrand of DFRobot sensor (or DFRobot is a rebrand of this one?)
#define SENSOR_SDS011 2
#define SENSOR_GROVE 3

// Shield pins
#ifdef ESP8266
#define PIN_SERIAL_TX D6
#define PIN_SERIAL_RX D7
#else
#define PIN_SERIAL_TX 7
#define PIN_SERIAL_RX 8
#endif

#include "Arduino.h"
#include "hackair_errors.h"
#include <SoftwareSerial.h>

/**
* Structure for storing & sending hackAir-related data to a
* server
*/
struct hackAirData {
float pm25;   /**< Amount of PM2.5 */
float pm10;   /**< Amount of PM1.0 */
int battery;  /**< Battery level */
int tamper;   /**< Tamper indicator */
int error;    /**< Error status */
};

class hackAIR {
public:
    // Constructors
    hackAIR(int sensor);

    /**
     * Initialize the sensor (pin modes, buses, etc)
     */
    void begin();

    /**
     * Read data from the sensor.
     */
    void readData(hackAirData &data);

    /**
     * Read data from the sensor n times and return the average reading.
     */
    void readAverageData(hackAirData &data, uint8_t n);

    /**
     * Empties a hackAir data structure
     */
    void clearData(hackAirData &data);

    /**
     * Uses pin A2 to control the sensor power switch (FET)
     */
    void enablePowerControl();

    /**
     * Turn the sensor power on
     */
    void turnOn();

    /**
     * Turn the sensor power off
     */
    void turnOff();

    /**
     * Compensates the measurements for the given humidity
     */
    void humidityCompensation(hackAirData &data, float humidity);

private:
    int _sensorType;
    long _lastTime;
    long _pulseDuration;
    unsigned char _buff[32];
};

#endif
