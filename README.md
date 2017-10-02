![hackAIR banner with logo](banner.png)

hackAIR is an EU-funded project aiming to develop an open technology platform for citizen observatories on air quality. Air pollution is an environmental issue with serious health and lifespan implications. However, it remains difficult for citizens to assess their exposure to air pollution and air quality issues in their country. Official air quality sensors are often few and far between, coverage is poor outside cities, and their data is not always easily accessible.

## hackAIR Arduino Library

The hackAIR project provides resources to create your own data collection station using the Arduino platform. The library in this repository aims to support a variety of air quality sensors so anyone with some electronics knowledge can measure air quality at home and optionally submit data to the hackAIR database.

### Why Arduino

> Arduino is an open-source electronics platform based on easy-to-use hardware and software. It's intended for anyone making interactive projects. *-arduino.cc*

The Arduino platform helps create electronic project easy and fun with the only prerequisite being willing to learn and experiment. The hackAIR library takes care of the details so you can get measuring in no time.

### Sending data

While not mandatory, the library provides helpers to send air quality data over the Internet to the hackAIR platform. The official [Ethernet Shield](https://www.arduino.cc/en/Main/ArduinoEthernetShieldV2) is supported alongside the [hackAIR WiFi shield](https://github.com/hackair-project/hackAir-ArduinoWiFi), a DIY option for adding wireless connectivity to your data collection station.

### Supported Sensors

The following sensors are currently supported by the library:
 * [DFRobot PM2.5 Air Quality sensor](https://hackair-project.github.io/hackAir-Arduino/sensors/dfrobot/)
 * [PMS5003](https://hackair-project.github.io/hackAir-Arduino/sensors/pms5003/)
 * [SDS011](https://hackair-project.github.io/hackAir-Arduino/sensors/sds011/)
 * [PPD42NS - Grove sensor by SEEDSTUDIO](https://hackair-project.github.io/hackAir-Arduino/sensors/grove/)

### Documentation

Official documentation is available at the [hackAIR-Arduino wiki](https://hackair-project.github.io/hackAir-Arduino/general/). The website is continiously updated with more examples and helpful info.

### Licensing

```
hackAIR Arduino Library
Copyright © 2016-2017 hackAir Consortium

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```
