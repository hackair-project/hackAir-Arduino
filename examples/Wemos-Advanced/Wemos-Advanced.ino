/**
 * @file Wemos - Advanced Example
 * This example reads data from a sensor and sends it to the hackAIR platform
 * using the Wemos integrated WiFi on a configurable frequency. This code
 * assumes a DHT11 humidity sensor connected to pin 2.
 *
 * @author Ilias Stavrakas
 * @author Thanasis Georgiou (Cleanup)
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <hackair.h>

// Replace the string below with your Authorization Token from the hackAIR
// platform
#define AUTHORIZATION "AUTHORIZATION TOKEN HERE"

// Sensor initialization
hackAIR sensor(SENSOR_SDS011);

// Setup the humidity sensor (pin 2)
DHT dht(2, DHT11);

// How often to measure (in minutes)
const unsigned long minutes_time_interval = 120;

// Setup ADC to measure Vcc (battery voltage)
ADC_MODE(ADC_VCC);

// Create a secure client for sending data using HTTPs
WiFiClientSecure client;

// Struct for storing sensor data
struct hackAirData data;

unsigned long previous_millis = 0;
void setup() {
  // Open serial communications
  Serial.begin(9600);

  // Initialize the PM sensor
  sensor.begin();
  sensor.enablePowerControl();
  sensor.turnOn();

  sensor.clearData(data);

  // Initialize temperature and humidity sensor
  dht.begin();

  // Initialize the WiFi connection
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("hackAIR-AP")) {
    Serial.println("failed to connect, please push reset button and try again");
    delay(3000);
    ESP.reset();
    delay(10000);
  }
  // check if we have connected to the WiFi
  Serial.println("network connected:)");
  Serial.println("Your local ip is:");
  Serial.println(WiFi.localIP());
}

void loop() {
  float vdd = ESP.getVcc() / 500.0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Measure data
  sensor.clearData(data);
  sensor.refresh(data);

  // Average readings (60 measurments)
  double pm25 = data.pm25;
  double pm10 = data.pm10;
  int error = 0;
  for (int i = 0; i < 59; i++) {
    // Read from the sensor
    sensor.refresh(data);

    // If error is not zero something went wrong with this measurment
    // and we should not send it.
    if (data.error == 0) {
      pm25 = (pm25 + data.pm25) / 2;
      pm10 = (pm10 + data.pm10) / 2;
    } else {
      error++;
    }
    delay(1000);  // Wait one second
  }
  data.pm25 = pm25;
  data.pm10 = pm10;
  data.error = error;

  // Measure humidity and temperature
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  
  // Send the data to the hackAIR server
  String dataJson = "{\"reading\":{\"PM2.5_AirPollutantValue\":\"";
  dataJson += pm25;
  dataJson += "\",\"PM10_AirPollutantValue\":\"";
  dataJson += pm10;
  dataJson += "\"},\"battery\":\"";
  dataJson += vdd;
  dataJson += "\",\"tamper\":\"";
  dataJson += "0";
  dataJson += "\",\"error\":\"";
  dataJson += "0";
  dataJson += "\"}";
  if (client.connect("api.hackair.eu", 443)) {
    Serial.println("connected");
    client.print("POST /sensors/arduino/measurements HTTP/1.1\r\n");
    client.print("Host: api.hackair.eu\r\n");
    client.print("Connection: close\r\n");
    client.print("Authorization: ");
    client.println(Authorization);
    client.print("Accept: application/vnd.hackair.v1+json\r\n");
    client.print("Cache-Control: no-cache\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: ");
    client.println(dataJson.length() + 2);
    client.println("");
    client.println(dataJson);
    Serial.println(dataJson);
    delay(500);
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    client.stop();
  }
  delay(1000);

  // Turn off sensor and go to sleep
  sensor.turnOff();
  unsigned long current_millis = millis();
  while (current_millis <
         (previous_millis + (minutes_time_interval * 60 * 1000))) {
    delay(10000);
    current_millis = millis();
    Serial.println(current_millis);
  }
  previous_millis = current_millis;
  sensor.turnOn();
}
