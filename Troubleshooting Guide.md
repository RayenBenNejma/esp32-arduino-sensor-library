Troubleshooting Guide
Problem: Getting 0 or no reading
Solutions:

Check wiring connections (especially TRIG and ECHO)
Verify 5V power supply
Ensure ECHO pin returns to LOW after measurement
Add small delay between measurements (50-100ms)
Check if object is too close (< 2cm)

Problem: Erratic/jumping readings
Solutions:

Surface is too soft/absorptive (carpet, foam)
Object is at an angle (not perpendicular)
Use moving average or median filter
Increase measurement distance
Ensure stable mounting

Problem: Reading always shows maximum
Solutions:

No object in range (> 4m)
Object is sound-absorbing material
Sensor damaged or fake module
Check timeout value in pulseIn()

Problem: ESP32 damage or not working
Solutions:

MUST use voltage divider on ECHO pin!
HC-SR04 outputs 5V on ECHO
ESP32 GPIO is 3.3V only
Use 1kΩ + 2kΩ voltage divider
Or use HC-SR04+ (3.3V compatible version)

Problem: Interference between multiple sensors
Solutions:

Trigger sensors in sequence, not simultaneously
Add 50-100ms delay between sensors
Use different frequencies (if available)
Ensure sensors don't "see" each other

