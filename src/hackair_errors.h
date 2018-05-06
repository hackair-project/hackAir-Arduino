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
* @file Error codes
* @author Thanasis Georgiou
*/

/** No error, everything is fine */
#define H_NO_ERROR 0

/** There was an error reading from the air quality sensor */
#define H_ERROR_SENSOR (1 << 0)

/**
 * There was an error reading from the sensor at least once
 * but the reading should be correct
 */
#define H_ERROR_SENSOR_ONCE (1 << 1)

/** There was an error reading from the humidity sensor */
#define H_ERROR_HUMIDITY (1 << 2)

/** Something else went wrong */
#define H_ERROR_UNKNOWN (1 << 3)