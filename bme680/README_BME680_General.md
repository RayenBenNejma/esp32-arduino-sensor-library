# BME680 Sensor — General Overview

## What is the BME680?

The BME680 is a multi-sensor module developed by Bosch Sensortec. It measures four environmental parameters in a single compact chip:

- **Temperature** — range: -40°C to +85°C, accuracy ±1°C
- **Humidity** — range: 0–100% RH, accuracy ±3%
- **Pressure** — range: 300–1100 hPa
- **Gas / Air Quality (VOC)** — measures volatile organic compounds (VOCs)

## How It Works

The gas sensor inside the BME680 works by heating a metal oxide element. When VOCs contact the hot surface, they react and change the electrical resistance. A lower resistance means more VOCs are present (worse air quality). A higher resistance means cleaner air.

> ⚠️ The sensor requires a **48-hour burn-in period** before giving stable gas readings.

## Pinout

| Pin | Description |
|-----|-------------|
| VCC | Power supply (1.8V – 3.6V) |
| GND | Ground |
| SDA | I2C Data |
| SCL | I2C Clock |
| SDO | I2C address select (LOW = 0x76, HIGH = 0x77) |
| CSB | Chip select (HIGH for I2C mode) |

## Communication Protocols

The BME680 supports two protocols:
- **I2C** — uses 2 wires (SDA, SCL), simple and commonly used
- **SPI** — uses 4 wires, faster but more complex

## Applications

### Home Automation
- Automatic ventilation control
- Air purifier triggering
- HVAC optimization

### Health & Wellness
- Indoor air quality monitoring
- Breath analysis
- Allergen detection

### Industrial
- Clean room monitoring
- Manufacturing quality control
- Laboratory environmental control

## Troubleshooting

**High gas resistance readings:**
- Sensor is new (needs burn-in time)
- Very clean environment
- Allow 48-hour burn-in period

**Low gas resistance readings:**
- VOCs present in air
- Recent cooking or cleaning
- Check ventilation

**Unstable readings:**
- Inadequate warm-up time
- Power supply issues
- Add 0.1µF capacitor near sensor

## Required Libraries

- `Adafruit BME680 Library` by Adafruit
- `Adafruit Unified Sensor` by Adafruit
