PIR Sensor – Passive Infrared Motion Detector

Overview
The PIR (Passive Infrared) sensor detects motion by measuring changes in infrared radiation emitted by objects. It is used in smart home systems, security applications, and energy-efficient automation.

How It Works
- Two pyroelectric elements detect infrared radiation.
- When a warm object moves across detection zones, differential signals generate voltage changes.
- The output signal triggers connected devices.

Key Features
Operating Voltage: 4.5V–20V DC
Detection Range: 3–7 meters
Detection Angle: 110° horizontal
Output Type: Digital (HIGH/LOW)
Warm-up Time: ~60 seconds
Adjustable Controls: Sensitivity & Delay Time potentiometers
Trigger Modes: Single / Repeatable

Applications
- Smart lighting systems
- Security alarms
- Energy-saving HVAC control
- Smart cameras
- Pet-immune detection setups

Wiring Example
HC-SR501 → Arduino UNO
VCC → 5V
GND → GND
OUT → Digital Pin 2
Optional LED → Pin 13 (through 220Ω resistor)

Troubleshooting Guide

Problem: False triggers / Random detections
Solutions:
- Increase warm-up time (60+ seconds)
- Adjust sensitivity potentiometer
- Check for heat sources (heaters, direct sunlight)
- Ensure stable power supply
- Mount away from air vents
- Shield from drafts

Problem: Not detecting motion
Solutions:
- Check wiring connections
- Verify power supply (4.5V–20V)
- Adjust sensitivity potentiometer (turn clockwise)
- Remove any obstructions from lens
- Test at optimal range (2–7 meters)
- Allow proper warm-up time

Problem: Triggers on pets
Solutions:
- Mount sensor higher (2m+) angled downward
- Reduce sensitivity
- Use pet-immune PIR sensors
- Adjust detection angle
- Consider sensor with adjustable zones

Problem: Output stays HIGH continuously
Solutions:
- Set jumper to non-repeatable mode
- Reduce delay time potentiometer
- Check for stuck relay/component
- Verify not in repeatable trigger mode
- Reset sensor (power cycle)

Problem: Short detection range
Solutions:
- Clean Fresnel lens
- Increase sensitivity potentiometer
- Check for obstructions
- Verify proper mounting height
- Replace if lens is damaged
