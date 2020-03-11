<a href="https://www.hardwario.com/"><img src="https://www.hardwario.com/ci/assets/hw-logo.svg" width="200" alt="HARDWARIO Logo" align="right"></a>

# Firmware for HARDWARIO Sigfox CO2 Monitor

[![Travis](https://img.shields.io/travis/bigclownlabs/bcf-sigfox-co2-monitor/master.svg)](https://travis-ci.org/bigclownlabs/bcf-sigfox-co2-monitor)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcf-sigfox-co2-monitor.svg)](https://github.com/bigclownlabs/bcf-sigfox-co2-monitor/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcf-sigfox-co2-monitor.svg)](https://github.com/bigclownlabs/bcf-sigfox-co2-monitor/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/hardwario_en.svg?style=social&label=Follow)](https://twitter.com/hardwario_en)

![sigfox-CO2-monitor-with-enclosure](images/sf-co2-enclosure.png)

This repository contains firmware for Sigfox CO2 Monitor.
Firmware is programmed into [Core Module](https://shop.bigclown.com/core-module).
Binary version is available in section [Releases](https://github.com/bigclownlabs/bcf-kit-sigfox-co2-monitor/releases).

**TODO** > Detailed information about this project can be found in [HARDWARIO Documentation](https://doc.bigclown.com).

**TODO** Link to documentation article

## Introduction

Sigfox CO2 Monitor is a battery-operated indoor device integrating these sensors:

* **CO2 Sensor** (carbon dioxide)
* **Thermometer** (ambient temperature)
* **Hygrometer** (relative air humidity)
* **Barometer** (atmospheric pressure)

This device is able to run from for AAA Alkaline batteries for at least 1 year.
It reports measured data to Sigfox network every 15 minutes.
You can route sensor data as HTTP POST request with JSON body to your own web app via [MySigfox](https://www.mysigfox.com) service.

## Hardware

The following hardware components are used for this project:

* **[CO2 Module](https://shop.bigclown.com/co2-module)**
* **[Temperature Tag](https://shop.bigclown.com/temperature-tag)**
* **[Humidity Tag](https://shop.bigclown.com/humidity-tag)**
* **[Barometer Tag](https://shop.bigclown.com/barometer-tag)**
* **[Sigfox Module](https://shop.bigclown.com/sigfox-module)**
* **[Core Module](https://shop.bigclown.com/core-module)**
* **[Battery Module](https://shop.bigclown.com/battery-module)**

**TODO** Link to enclosure cad files.

![sigfox-CO2-monitor](images/sf-co2.png)

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with &#x2764;&nbsp; by [**HARDWARIO s.r.o.**](https://www.hardwario.com/) in the heart of Europe.
