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
 * @file Header for WiFi-specific code
 * @author Thanasis Georgiou
 */

#ifndef hackAIRwifi_h
#define hackAIRwifi_h

#define ESP_PIN_RST 2
#define ESP_PIN_CS 4

void wifi_begin();
void wifi_setToken(const String &token);
void wifi_sendData(hackAirData &data);
void wifi_waitForReady();
void wifi_clearAPs();
void wifi_reset();
void wifi_hardReset();
void wifi_powerOff();
void wifi_powerOn();

#endif
