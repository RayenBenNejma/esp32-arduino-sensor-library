# BME680 + Arduino — Setup Guide

## Hardware Requirements

- Arduino Uno / Mega / Nano
- BME680 sensor module
- Jumper wires
- USB cable

## Wiring (I2C)

| BME680 | Arduino Uno | Arduino Mega |
|--------|-------------|--------------|
| VCC    | 3.3V        | 3.3V         |
| GND    | GND         | GND          |
| SDA    | A4          | PIN 20       |
| SCL    | A5          | PIN 21       |
| SDO    | GND (0x76)  | GND (0x76)   |
| CSB    | 3.3V        | 3.3V         |

> ⚠️ Arduino Uno outputs 5V on I2C pins — use a **logic level converter** or a BME680 module with built-in voltage regulator.

## Required Libraries

Install from Arduino IDE → Library Manager:
- `Adafruit BME680 Library`
- `Adafruit Unified Sensor`

## Recommended Sensor Settings

| Parameter | Setting |
|-----------|---------|
| Temperature oversampling | OS_8X |
| Humidity oversampling | OS_2X |
| Pressure oversampling | OS_4X |
| IIR Filter | SIZE_3 |
| Gas heater | 320°C for 150ms |

## Reading Explanation

| Parameter | Unit | Good Value |
|-----------|------|------------|
| Temperature | °C | 18–26°C (room) |
| Humidity | % | 40–60% (comfortable) |
| Pressure | hPa | ~1013 hPa (sea level) |
| Gas Resistance | KΩ | >50 KΩ (clean air) |

## Air Quality Scale

| Gas Resistance | Air Quality |
|----------------|-------------|
| > 50 KΩ | Excellent |
| 20–50 KΩ | Good |
| 10–20 KΩ | Moderate |
| < 10 KΩ | Poor |

## Applications

### Home Automation
- Automatic ventilation control
- Air purifier triggering
- HVAC optimization

### Health & Wellness
- Indoor air quality monitoring
- Allergen detection

### Industrial
- Clean room monitoring
- Laboratory environmental control

## Troubleshooting

**"BME680 not found":**
- Check A4 (SDA) and A5 (SCL) connections
- Try address 0x77 if 0x76 fails
- Use logic level converter if powering from 5V Arduino

**performReading() returns false:**
- Increase delay between readings
- Check 3.3V power supply

**Gas readings unstable:**
- New sensor needs 48-hour burn-in period
- Keep sensor away from direct air flow
- Allow 5-minute warm-up before reading gas

**High current draw:**
- Gas heater uses ~12mA — this is normal
- Use external 3.3V regulator for stable readings
