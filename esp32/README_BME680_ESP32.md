# BME680 + ESP32 — Setup Guide

## Hardware Requirements

- ESP32 development board
- BME680 sensor module
- Jumper wires
- USB cable

## Wiring (I2C)

| BME680 | ESP32 |
|--------|-------|
| VCC    | 3.3V  |
| GND    | GND   |
| SDA    | GPIO 21 |
| SCL    | GPIO 22 |
| SDO    | GND (address 0x76) |
| CSB    | 3.3V  |

> ⚠️ Do NOT connect VCC to 5V — the BME680 runs on 3.3V only.

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
- Check SDA/SCL wiring
- Try address 0x77 instead of 0x76
- Make sure VCC = 3.3V not 5V

**performReading() returns false:**
- Add delay before reading
- Check power supply stability

**Gas readings unstable:**
- Normal for new sensor — wait 48 hours burn-in
- Keep away from direct air flow
- Allow 5-minute warm-up before reading gas
