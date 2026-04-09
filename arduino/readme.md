## Installation Instructions

### Arduino:
1. Install Adafruit BME280 library via Library Manager
2. Install Adafruit Unified Sensor library
3. Upload code to Arduino
4. Open Serial Monitor (9600 baud)
## Troubleshooting

**Problem:** Sensor not found
- Check I2C address (0x76 or 0x77)
- Verify wiring connections
- Use I2C scanner sketch

**Problem:** Invalid readings (NaN)
- Check power supply (3.3V, not 5V!)
- Verify proper connections
- Add 10kΩ pull-up resistors to SDA/SCL if needed

**Problem:** Unstable readings
- Add 0.1µF capacitor between VCC and GND
- Keep wires short
- Use shielded cables for long distances

