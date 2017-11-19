/**
 * @file hackAIR WiFi Shield Example - Reset Access Point
 * 
 * This example resets the WiFi shield's saved access point so the user
 * can set it up again.
 *
 * This example is part of the hackAIR Arduino Library and is available
 * in the Public Domain.
*/

#include "hackair.h"
#include "hackair_wifi.h"

void setup() {
  wifi_begin();
  wifi_clearAPs();
}

void loop() {
  // Do nothing
}