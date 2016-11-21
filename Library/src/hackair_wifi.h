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

#ifndef hackAIRwifi_h
#define hackAIRwifi_h

#define ESP_PIN_RST 2
#define ESP_PIN_CS 4

void wifi_begin();
void wifi_setToken(const String &token);
void wifi_sendData(float pm25, float pm10, int battery, int tamper, int error);
void wifi_waitForReady();
void wifi_clearAPs();
void wifi_reset();
void wifi_hardReset();
void wifi_powerOff();
void wifi_powerOn();

#endif
