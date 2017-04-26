 /**
 * @file LCD node example
 * This example reads data from a sensor (default choice is the serial SDS011)
 * and displays it on an LCD.
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
 */
#include <LiquidCrystal.h>
#include <hackair.h>

// Specify your sensor
hackAIR sensor(SENSOR_DFROBOT);

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Initialize the sensor
  sensor.begin();

  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  // Take a reading
  struct hackAirData data;
  sensor.refresh(data);

  // Clear screen and move cursor to the top left
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // If it was invalid, print error
  if (data.error != 0) {
    lcd.print("Error!");
  } else {
    // Print the values to the LCD
    lcd.print("PM10 :");
    lcd.print(data.pm10);
    lcd.setCursor(0, 1);
    lcd.print("PM2.5:");
    lcd.print(data.pm25);
  }

  // Wait a bit
  delay(2500);
}
