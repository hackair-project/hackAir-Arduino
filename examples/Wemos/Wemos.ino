/**
 * @file Wemos - Advanced Example
 * This example reads data from a sensor and sends it to the hackAIR platform
 * using the Wemos integrated WiFi . This code
 * assumes a DHT11 humidity sensor connected to pin D4.
 *
 * @author LoRAthens Air Quality team
 * @author Thanasis Georgiou (Cleanup)
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */

#include <Arduino.h>
#include <DHT.h>  // Adafruit's DHT sensor library https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h>
#include <DNSServer.h>  // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>  // Local WebServer used to serve the configuration portal
#include <ESP8266WiFi.h>  // ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <FS.h>  //this needs to be 2nd. Don't change it...
#include <WiFiClientSecure.h>  // Variant of WiFiClient with TLS support (from ESP82266 core wifi)
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <hackair.h>      // https://github.com/hackair-project/hackAir-Arduino

// Replace the string below with your Authorization Token from the hackAIR
// platform
#define AUTHORIZATION "AUTHORIZATION TOKEN"

// Sensor initialization
hackAIR sensor(SENSOR_SDS011);

// Setup the humidity sensor (pin D4)
DHT dht(D4, DHT11);

// How often to measure (in minutes)
const unsigned long minutes_time_interval = 5; // minutes

// How long to try for a valid measurement
const unsigned long retry_period_s = 10; // s

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
  if (!wifiManager.autoConnect("ESP-wemos")) {
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

  Serial.println("Doing measurement");
  sensor.clearData(data);
  unsigned long timestamp = millis();
  while (data.error != 0) {
    if(millis() > timestamp + (retry_period_s * 1000)) {
      Serial.println("Tried too long, giving up.");
      break;
    }
    sensor.turnOn();
    delay(100);
    sensor.readAverageData(data, 60); // 60 averages
    yield();
  }

  // Compensate for humidity
  float humidity = dht.readHumidity();
  if (data.error == 0 && !isnan(humidity)) {
    sensor.humidityCompensation(data, humidity);
  }

  // Send the data to the hackAIR server
  String dataJson = "{\"reading\":{\"PM2.5_AirPollutantValue\":\"";
  dataJson += data.pm25;
  dataJson += "\",\"PM10_AirPollutantValue\":\"";
  dataJson += data.pm10;
  dataJson += "\"},\"battery\":\"";
  dataJson += vdd;
  dataJson += "\",\"tamper\":\"";
  dataJson += data.tamper;
  dataJson += "\",\"error\":\"";
  dataJson += data.error;
  dataJson += "\"}";
  if (client.connect("api.hackair.eu", 443)) {
    Serial.println("connected");
    client.print("POST /sensors/arduino/measurements HTTP/1.1\r\n");
    client.print("Host: api.hackair.eu\r\n");
    client.print("Connection: close\r\n");
    client.print("Authorization: ");
    client.println(AUTHORIZATION);
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
  
  // Turn off sensor and go to sleep
  sensor.turnOff();
  Serial.println("");
  Serial.println("Sensor Off");
  unsigned long current_millis = millis();
  while (current_millis <
         (previous_millis + (minutes_time_interval * 60 * 1000))) {
    delay(10000);
    current_millis = millis();
    Serial.print(".");
  }
  previous_millis = current_millis;
}