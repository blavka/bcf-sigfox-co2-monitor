#!/usr/bin/env python3
import sys
import __future__


def decode(data):
    if len(data) != 24:
        raise Exception("Bad data length, 24 characters expected")

    header = int(data[0:2], 16)

    ret = {
        "temperature": None,
        "humidity": None,
        "pressure": None,
        "co2_concentration_avg": None,
        "co2_concentration_median": None,
        "co2_concentration_raw": None
    }

    if header & 0x01:
        ret["temperature"] = int(data[4:6] + data[2:4], 16) / 2.0

    if header & 0x02:
        ret["humidity"] = int(data[6:8], 16) / 2.0

    if header & 0x04:
        ret["pressure"] = int(data[10:12] + data[8:10], 16) * 2.0

    if header & 0x08:
        ret["co2_concentration_avg"] = int(data[14:16] + data[12:14], 16)

    if header & 0x10:
        ret["co2_concentration_median"] = int(data[18:20] + data[16:18], 16)

    if header & 0x20:
        ret["co2_concentration_raw"] = int(data[22:24] + data[20:22], 16)

    return ret


def pprint(data):
    print('Temperature :', data['temperature'], "°C")
    print('Humidity :', data['humidity'], "%")
    print('Pressure :', data['pressure'] / 100.0, "hPa")
    print('CO2 avg :', data['co2_concentration_avg'], "ppm")
    print('CO2 median :', data['co2_concentration_median'], "ppm")
    print('CO2 raw :', data['co2_concentration_raw'], "ppm")


if __name__ == '__main__':
    if len(sys.argv) != 2 or sys.argv[1] in ('help', '-h', '--help'):
        print("usage: python3 decode.py [data]")
        print("example: python3 decode.py 3d130000f6c09a0198019401")
        exit(1)

    data = decode(sys.argv[1])
    pprint(data)
