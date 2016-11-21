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

#include "Arduino.h"
#include "hackair.h"

SoftwareSerial _serial(8, 7);

/* Constructors */
hackAIR::hackAIR(int sensorType) {
	// Initialize last reading
	_sensorType = sensorType;
}

void hackAIR::begin() {
	_serial.begin(9600);
}

int hackAIR::refresh() {
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
                _pm10 = ((_buff[8] << 8) + _buff[9]);
                _pm25 = ((_buff[6] << 8) + _buff[7]);
                _pm01 = ((_buff[4] << 8) + _buff[5]);
                
                return 0;
            } else {
                _pm10 = -1;
                _pm25 = -1;
                _pm01 = -1;
                
                return 1;
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
                // Package is OK
                _pm25 = ((_buff[3] << 8) + _buff[2]);
                _pm10 = ((_buff[5] << 8) + _buff[4]);
                _pm01 = -1;
                
                return 0;
            } else {
                _pm10 = -1;
                _pm25 = -1;
                _pm01 = -1;
                
                return 1;
            }
        }
    }
}

float hackAIR::readPM25() {
    if (_sensorType == SENSOR_SDS011) {
        return _pm25 / 10;
    } else {
        return _pm25;
    }    
}

float hackAIR::readPM10() {
    if (_sensorType == SENSOR_SDS011) {
        return _pm10 / 10;
    } else {
        return _pm10;
    }    
}

float hackAIR::readPM01() {
    if (_sensorType == SENSOR_SDS011) {
        return _pm01 / 10;
    } else {
        return _pm01;
    }    
}