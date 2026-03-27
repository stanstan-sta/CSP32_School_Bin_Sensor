# Hardware Specifications

## CSP32 School Bin Sensor - Bill of Materials (BOM)

### Core Components

| Component | Model | Quantity | Est. Price (USD) | Notes |
|-----------|-------|----------|------------------|-------|
| Microcontroller | ESP32 DevKit V1 | 1 | $4-6 | Main processing unit with WiFi |
| Ultrasonic Sensor | HC-SR04 | 1 | $1-2 | Distance measurement (2-400cm range) |
| Power Supply | 5V 2A USB Adapter | 1 | $2-3 | Power source |
| USB Cable | Micro USB Cable | 1 | $1-2 | Power and programming |
| Enclosure | Waterproof Box | 1 | $3-5 | Protection from elements |
| **Total** | | | **$11-18** | **Budget-friendly!** |

### Optional Components (for enhanced functionality)

| Component | Purpose | Est. Price (USD) |
|-----------|---------|------------------|
| Temperature Sensor (DHT22) | Monitor ambient temperature | $3-5 |
| LED Indicators (RGB) | Visual status indication | $1-2 |
| Buzzer | Audio alerts when bin is full | $0.50-1 |
| Battery + Solar Panel | Off-grid operation | $15-25 |

## Pin Configuration

### ESP32 Pin Connections

```
ESP32 DevKit V1 Pin Layout
┌────────────────────────┐
│                        │
│   HC-SR04 Connections: │
│   GPIO5  -> TRIG       │
│   GPIO18 -> ECHO       │
│   5V     -> VCC        │
│   GND    -> GND        │
│                        │
│   Built-in:            │
│   GPIO2  -> LED        │
│                        │
└────────────────────────┘
```

### Detailed Pin Mapping

| ESP32 Pin | Function | Connected To |
|-----------|----------|--------------|
| GPIO5 | Digital Output | HC-SR04 TRIG |
| GPIO18 | Digital Input | HC-SR04 ECHO |
| GPIO2 | Digital Output | Built-in LED |
| 5V | Power Supply | HC-SR04 VCC |
| GND | Ground | HC-SR04 GND |

## Circuit Diagram

```
                    ESP32 DevKit V1
                    ┌─────────────┐
                    │             │
        ┌───────────┤ GPIO5       │
        │           │             │
        │      ┌────┤ GPIO18      │
        │      │    │             │
        │      │ ┌──┤ 5V          │
        │      │ │  │             │
        │      │ │ ┌┤ GND         │
        │      │ │ ││             │
        │      │ │ │└─────────────┘
        │      │ │ │
        │      │ │ │
    ┌───▼──────▼─▼─▼──┐
    │                  │
    │    HC-SR04       │
    │  TRIG ECHO V G   │
    │                  │
    │   Ultrasonic     │
    │     Sensor       │
    └──────────────────┘
```

## Physical Installation Guide

### 1. Sensor Placement

- Mount the HC-SR04 sensor **inside the bin**, facing downward
- Position at the **top center** of the bin for accurate readings
- Ensure sensor is **level and stable**
- Keep sensor at least **5cm from the bin rim**
- Avoid placing near bin edges or corners

### 2. Mounting Options

**Option A: Top-Mount (Recommended)**
```
    Bin Lid
    ┌─────────────┐
    │   [Sensor]  │ ← HC-SR04 mounted on underside of lid
    │             │
    │             │
    │    ░░░░░    │ ← Waste level
    │    ░░░░░    │
    │    ░░░░░    │
    └─────────────┘
```

**Option B: Side-Mount**
```
    Bin Wall
    │
    │ [Sensor]  ← Angled downward
    │    ╲
    │     ╲
    │      ↓
    │    ░░░░░  ← Waste level
    │    ░░░░░
    └─────────
```

### 3. Weatherproofing

- Use waterproof enclosure for ESP32
- Seal cable entry points with silicone
- Position enclosure to avoid direct rain/sun exposure
- Consider ventilation to prevent condensation

### 4. Power Considerations

**Option A: Mains Power**
- Use 5V USB adapter
- Run cable to nearest power outlet
- Consider cable management and safety

**Option B: Battery Power**
- Use 18650 battery pack (2S/3S configuration)
- Add solar panel for continuous operation
- Implement deep sleep mode for power saving

## Calibration Procedure

1. **Measure actual bin height** (in centimeters)
2. **Measure sensor mounting distance** from bin rim
3. **Update config.h**:
   ```cpp
   #define BIN_HEIGHT 100        // Your actual bin height
   #define SENSOR_OFFSET 5       // Sensor distance from rim
   ```
4. **Test with empty bin**:
   - Should read close to BIN_HEIGHT
   - Adjust SENSOR_OFFSET if needed
5. **Test with full bin**:
   - Should read close to 0% empty
   - Verify threshold levels are appropriate

## Troubleshooting

### Common Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| No readings | Wiring incorrect | Check pin connections |
| Erratic readings | Sensor too close to edge | Reposition to center |
| Always reads "FULL" | Sensor upside down | Flip sensor orientation |
| WiFi won't connect | Wrong credentials | Connect to config AP and reconfigure |
| Constant ERROR status | Obstacle in sensor path | Clear line of sight to waste |

## Safety Notes

⚠️ **Important Safety Information**

- **Electrical Safety**: Use proper insulation and waterproofing
- **Fire Hazard**: Do not install near heat sources or flammable materials
- **Installation**: Secure all components to prevent falling into bin
- **Maintenance**: Periodically clean sensor lens for accurate readings
- **Disposal**: Follow proper e-waste disposal procedures when decommissioning

## Maintenance Schedule

| Interval | Task |
|----------|------|
| Weekly | Visual inspection of sensor and wiring |
| Monthly | Clean sensor lens with soft cloth |
| Quarterly | Check battery voltage (if battery-powered) |
| Annually | Replace enclosure seals if needed |

---

**Document Version:** 1.0.0
**Last Updated:** 2026-03-13
**Project:** CSP32 School Bin Sensor
