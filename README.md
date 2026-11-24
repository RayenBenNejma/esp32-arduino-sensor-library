# ESP32 Sensor Library

Ready-to-flash example code for common sensors on **ESP32** and **Arduino**. Each sensor has its own folder with a sketch for each board, a wiring table and sample output.

![Platform](https://img.shields.io/badge/platform-ESP32%20%7C%20Arduino-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-informational)
![License](https://img.shields.io/badge/license-MIT-green)

## Sensors

| Sensor | Measures | Interface | Folder |
| --- | --- | --- | --- |
| **BME280** | Temperature, humidity, pressure | I2C | [`bme280/`](bme280) |
| **BME680** | Temperature, humidity, pressure, gas resistance (air quality) | I2C | [`bme680/`](bme680) |
| **HC-SR04** | Distance (ultrasonic) | Digital (TRIG / ECHO) | [`hc-sr04/`](hc-sr04) |
| **PIR HC-SR501** | Motion / presence | Digital | [`pir-hc-sr501/`](pir-hc-sr501) |

## Repository structure

```
esp32-sensor-library/
├── bme280/
│   ├── README.md          # wiring, libraries, sample output
│   ├── esp32/
│   └── arduino/
├── bme680/
├── hc-sr04/
├── pir-hc-sr501/
├── LICENSE
└── README.md
```

## Getting started

1. Clone the repo:
   ```bash
   git clone https://github.com/RayenBenNejma/esp32-sensor-library.git
   ```
2. Open the sketch you need in the **Arduino IDE** (or PlatformIO). Open the `.ino` file inside its folder, since the Arduino IDE needs the folder and file names to match.
3. Install the libraries listed in that sensor's README (Tools → Manage Libraries).
4. Select your board (ESP32 Dev Module, Arduino Uno, etc.) and the correct port.
5. Wire the sensor using the table in its README, upload, and open the Serial Monitor at the baud rate shown in the sketch.

## Quick wiring notes

| Sensor | Power | Signal pins | Watch out for |
| --- | --- | --- | --- |
| BME280 / BME680 | 3.3 V | SDA, SCL (ESP32 default: GPIO 21 / 22) | I2C address is usually `0x76` or `0x77`. Run an I2C scanner if the sensor isn't found. |
| HC-SR04 | 5 V | TRIG, ECHO | ECHO outputs 5 V. Use a voltage divider (or a level shifter) before connecting it to an ESP32 pin. |
| PIR HC-SR501 | 5 V | OUT | The sensor needs about 30–60 s to warm up after power-on. Sensitivity, delay and trigger mode are set with the on-board potentiometers and jumper. |

Exact pins used in each sketch are in the per-sensor READMEs.

## Where these are used

These sensors are building blocks in my larger projects:

- [**Smart Classroom**](https://github.com/RayenBenNejma/smart-classroom): ESP32 with BME280 and PIR, plus LSTM temperature prediction
- [**STM32 Weather Station**](https://github.com/RayenBenNejma/weather-station-stm32): BME680 over I2C with a Flask dashboard

## Contributing

Found a bug or want to add another sensor? Open an issue or a pull request. Please keep the same structure: one folder per sensor, one README with wiring and sample output.

## License

Released under the [MIT License](LICENSE).

## Author

**Rayen Ben Nejma**: Embedded Systems & IoT student, ISET Bizerte, Tunisia
[GitHub](https://github.com/RayenBenNejma) · [LinkedIn](https://www.linkedin.com/in/rayen-ben-nejma-ba1260321)
