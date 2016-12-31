/**
 * hackAIR Arduino Library
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

#ifndef hackAIR_h
#define hackAIR_h

// Sensors
#define SENSOR_DFROBOT 1
#define SENSOR_SDS011 2
#define SENSOR_GP2Y1010AU0F 3
#define SENSOR_DN7C3CA006 4

// Shield pins
#define PIN_SERIAL_TX 7
#define PIN_SERIAL_RX 8
#define PIN_IO_1 A0
#define PIN_IO_2 A1

#include "Arduino.h"
#include <SoftwareSerial.h>

class hackAIR {
public:
	// Constructors
	hackAIR(int sensor);

	/**
	 * Initialize the sensor (pin modes, buses, etc)
	 */
	void begin();
    
    /**
     * Read data from the sensor and process packages. Mainly used for digital/serial
     * sensors.
     */
    int refresh();

    /**
     * Get the value of 2.5um particles
     */
	float readPM25();
    
    /**
     * Get the value of 10um particles
     */
    float readPM10();
    
     /**
     * Get the value of 0.1um particles
     */
    float readPM01();
    
    /**
     * Get the raw reading value. For sensors that don't exactly measure particulate matter.
     */
    int readRaw();

private:
    int _sensorType;
    int _pm25;
    int _pm10;
    int _pm01;
    unsigned char _buff[32];
};

#endif
