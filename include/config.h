/**
 * CSP32 School Bin Sensor - Configuration Header
 *
 * Cheap (Cost-Effective) School Bin Monitoring Solution
 *
 * This file contains all configurable parameters for the sensor system.
 * Copy this to config_local.h and customize for your deployment.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// SENSOR CONFIGURATION
// ============================================================================

// Ultrasonic Sensor Pins (HC-SR04)
#define TRIG_PIN 5        // GPIO5 - Trigger pin
#define ECHO_PIN 18       // GPIO18 - Echo pin

// Sensor Reading Configuration
#define SENSOR_READ_INTERVAL 60000    // Read every 60 seconds (in milliseconds)
#define NUM_SAMPLES 5                 // Number of samples to average
#define SAMPLE_DELAY 100              // Delay between samples (ms)

// Bin Dimensions (in centimeters)
#define BIN_HEIGHT 100                // Total height of the bin
#define SENSOR_OFFSET 5               // Distance from sensor to bin top

// Thresholds for bin status
#define EMPTY_THRESHOLD 90            // > 90% empty
#define LOW_THRESHOLD 70              // > 70% empty
#define MEDIUM_THRESHOLD 40           // > 40% empty
#define HIGH_THRESHOLD 20             // > 20% empty
// Below HIGH_THRESHOLD = FULL

// ============================================================================
// WIFI CONFIGURATION
// ============================================================================

// WiFi credentials (use WiFiManager for setup)
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// WiFi connection timeout
#define WIFI_CONNECT_TIMEOUT 30000    // 30 seconds

// ============================================================================
// MQTT CONFIGURATION
// ============================================================================

#define MQTT_ENABLED true             // Enable/disable MQTT reporting
#define MQTT_SERVER "mqtt.example.com"
#define MQTT_PORT 1883
#define MQTT_USER "sensor_user"
#define MQTT_PASSWORD "sensor_password"

// MQTT Topics
#define MQTT_TOPIC_PREFIX "school/bin/"
#define MQTT_TOPIC_STATUS "status"
#define MQTT_TOPIC_DISTANCE "distance"
#define MQTT_TOPIC_PERCENTAGE "percentage"
#define MQTT_TOPIC_LEVEL "level"

// ============================================================================
// DEVICE CONFIGURATION
// ============================================================================

#define DEVICE_ID "BIN_001"           // Unique identifier for this sensor
#define FIRMWARE_VERSION "1.0.0"

// ============================================================================
// LED INDICATOR (Built-in LED)
// ============================================================================

#define LED_PIN 2                     // Built-in LED on ESP32
#define LED_BLINK_INTERVAL 1000       // Blink interval when active (ms)

// ============================================================================
// DEBUGGING
// ============================================================================

#define DEBUG_MODE true               // Enable serial debugging
#define SERIAL_BAUD 115200            // Serial baud rate

#endif // CONFIG_H
