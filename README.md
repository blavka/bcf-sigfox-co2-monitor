<a href="https://www.bigclown.com"><img src="https://s3.eu-central-1.amazonaws.com/bigclown/gh-readme-logo.png" alt="BigClown Logo" align="right"></a>

# Project Sigfox CO2 Monitor

[![Travis](https://img.shields.io/travis/bigclownlabs/bcp-sigfox-co2/master.svg)](https://travis-ci.org/bigclownlabs/bcp-sigfox-co2)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcp-sigfox-co2.svg)](https://github.com/bigclownlabs/bcp-sigfox-co2/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcp-sigfox-co2.svg)](https://github.com/bigclownlabs/bcp-sigfox-co2/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/BigClownLabs.svg?style=social&label=Follow)](https://twitter.com/BigClownLabs)

![sigfox-CO2-monitor-with-enclosure](images/sf-co2-enclosure.png)

This repository contains firmware for Sigfox CO2 Monitor.
Firmware is programmed into [Core Module](https://shop.bigclown.com/products/core-module).

**TODO** Binary version is available in section [Releases](https://github.com/bigclownlabs/bcp-sigfox-co2/releases).

**TODO** > Detailed information about this project can be found in [BigClown Documentation](https://doc.bigclown.com).

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

* **[CO2 Module](https://shop.bigclown.com/products/co2-module)**
* **[Temperature Tag](https://shop.bigclown.com/products/temperature-tag)**
* **[Humidity Tag](https://shop.bigclown.com/products/humidity-tag)**
* **[Barometer Tag](https://shop.bigclown.com/products/barometr-tag)**
* **[Sigfox Module](https://shop.bigclown.com/products/sigfox-module)**
* **[Core Module](https://shop.bigclown.com/products/core-module)**
* **[Battery Module](https://shop.bigclown.com/products/battery-module)**

**TODO** Link to enclosure cad files.

![sigfox-CO2-monitor](images/sf-co2.png)

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with ❤ by [BigClown Labs s.r.o.](https://www.bigclown.com) in Czech Republic.
