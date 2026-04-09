### ESP32:
1. Install ESP32 board support in Arduino IDE
2. Install Adafruit BME280 library
3. Select your ESP32 board
4. Upload code
5. Open Serial Monitor (115200 baud)

---

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

