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
 * @file Header for Ethernet-specific code. For use with the official ethernet shield.
 * @author Thanasis Georgiou
 */

#ifndef hackAIRethernet_h
#define hackAIRethernet_h

void ethernet_sendData(const String &token, float pm25, float pm10, int battery, int tamper, int error);

#endif
