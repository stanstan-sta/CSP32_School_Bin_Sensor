/**
 * CSP32 School Bin Sensor - Main Firmware
 *
 * A cost-effective solution for monitoring school waste bin fill levels
 * using an ESP32 microcontroller and HC-SR04 ultrasonic sensor.
 *
 * Features:
 * - Ultrasonic distance measurement
 * - WiFi connectivity with WiFiManager
 * - MQTT reporting for IoT integration
 * - Multi-level fill status (Empty, Low, Medium, High, Full)
 * - Power-efficient design with deep sleep support
 *
 * Hardware Requirements:
 * - ESP32 Development Board
 * - HC-SR04 Ultrasonic Sensor
 * - 5V Power Supply
 *
 * @author CSP32 Project Team
 * @version 1.0.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HCSR04.h>
#include "config.h"

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

// Ultrasonic sensor object
HCSR04 hcsr04(TRIG_PIN, ECHO_PIN);

// WiFi and MQTT clients
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// WiFi Manager for easy configuration
WiFiManager wifiManager;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

unsigned long lastSensorRead = 0;
unsigned long lastLedBlink = 0;
bool ledState = false;

enum BinLevel {
    EMPTY,
    LOW,
    MEDIUM,
    HIGH,
    FULL,
    ERROR
};

struct SensorData {
    float distance;           // Distance to waste surface (cm)
    float fillPercentage;     // Percentage of bin filled
    BinLevel level;           // Current fill level
    unsigned long timestamp;  // Reading timestamp
    bool valid;               // Reading validity flag
};

SensorData currentReading;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void setupSerial();
void setupPins();
void setupWiFi();
void setupMQTT();
void readSensor();
float getAverageDistance();
BinLevel calculateBinLevel(float fillPercentage);
String getLevelString(BinLevel level);
void publishMQTT();
void reconnectMQTT();
void blinkLED();
void printSensorData();

// ============================================================================
// SETUP FUNCTION
// ============================================================================

void setup() {
    setupSerial();
    setupPins();

    #if DEBUG_MODE
    Serial.println("\n\n=================================");
    Serial.println("CSP32 School Bin Sensor");
    Serial.println("Version: " + String(FIRMWARE_VERSION));
    Serial.println("Device ID: " + String(DEVICE_ID));
    Serial.println("=================================\n");
    #endif

    setupWiFi();

    #if MQTT_ENABLED
    setupMQTT();
    #endif

    // Initial sensor reading
    delay(1000);
    readSensor();

    #if DEBUG_MODE
    Serial.println("\nSetup complete. Starting main loop...\n");
    #endif
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
    unsigned long currentMillis = millis();

    // Handle MQTT connection
    #if MQTT_ENABLED
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();
    #endif

    // Read sensor at specified interval
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;
        readSensor();

        #if DEBUG_MODE
        printSensorData();
        #endif

        #if MQTT_ENABLED
        publishMQTT();
        #endif
    }

    // Blink LED to indicate activity
    blinkLED();

    // Small delay to prevent watchdog reset
    delay(10);
}

// ============================================================================
// SETUP FUNCTIONS
// ============================================================================

void setupSerial() {
    #if DEBUG_MODE
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    #endif
}

void setupPins() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void setupWiFi() {
    #if DEBUG_MODE
    Serial.println("Configuring WiFi...");
    #endif

    // Set WiFiManager timeout
    wifiManager.setConfigPortalTimeout(180);

    // Try to connect using saved credentials
    // If it fails, it starts an access point for configuration
    String apName = "CSP32_" + String(DEVICE_ID);

    #if DEBUG_MODE
    Serial.println("Connecting to WiFi...");
    Serial.println("If connection fails, connect to AP: " + apName);
    #endif

    if (!wifiManager.autoConnect(apName.c_str())) {
        #if DEBUG_MODE
        Serial.println("Failed to connect. Restarting...");
        #endif
        delay(3000);
        ESP.restart();
    }

    #if DEBUG_MODE
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    #endif
}

void setupMQTT() {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

    #if DEBUG_MODE
    Serial.println("MQTT configured");
    Serial.print("Server: ");
    Serial.print(MQTT_SERVER);
    Serial.print(":");
    Serial.println(MQTT_PORT);
    #endif
}

// ============================================================================
// SENSOR FUNCTIONS
// ============================================================================

void readSensor() {
    float avgDistance = getAverageDistance();

    // Validate reading
    if (avgDistance < 0 || avgDistance > 400) {  // HC-SR04 range: 2-400cm
        #if DEBUG_MODE
        Serial.println("ERROR: Invalid sensor reading!");
        #endif
        currentReading.valid = false;
        currentReading.level = ERROR;
        return;
    }

    // Calculate actual distance to waste (accounting for sensor offset)
    float actualDistance = avgDistance - SENSOR_OFFSET;

    // Calculate fill percentage
    float fillPercentage = 100.0 - ((actualDistance / BIN_HEIGHT) * 100.0);

    // Constrain to valid range
    fillPercentage = constrain(fillPercentage, 0.0, 100.0);

    // Calculate bin level
    BinLevel level = calculateBinLevel(fillPercentage);

    // Update current reading
    currentReading.distance = avgDistance;
    currentReading.fillPercentage = fillPercentage;
    currentReading.level = level;
    currentReading.timestamp = millis();
    currentReading.valid = true;
}

float getAverageDistance() {
    float sum = 0;
    int validSamples = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        float distance = hcsr04.dist();

        // Only include valid readings in average
        if (distance > 0 && distance < 400) {
            sum += distance;
            validSamples++;
        }

        if (i < NUM_SAMPLES - 1) {
            delay(SAMPLE_DELAY);
        }
    }

    if (validSamples == 0) {
        return -1;  // No valid samples
    }

    return sum / validSamples;
}

BinLevel calculateBinLevel(float fillPercentage) {
    float emptyPercentage = 100.0 - fillPercentage;

    if (emptyPercentage > EMPTY_THRESHOLD) return EMPTY;
    if (emptyPercentage > LOW_THRESHOLD) return LOW;
    if (emptyPercentage > MEDIUM_THRESHOLD) return MEDIUM;
    if (emptyPercentage > HIGH_THRESHOLD) return HIGH;
    return FULL;
}

String getLevelString(BinLevel level) {
    switch (level) {
        case EMPTY: return "EMPTY";
        case LOW: return "LOW";
        case MEDIUM: return "MEDIUM";
        case HIGH: return "HIGH";
        case FULL: return "FULL";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// ============================================================================
// MQTT FUNCTIONS
// ============================================================================

void reconnectMQTT() {
    static unsigned long lastAttempt = 0;
    unsigned long currentMillis = millis();

    // Try to reconnect every 5 seconds
    if (currentMillis - lastAttempt < 5000) {
        return;
    }

    lastAttempt = currentMillis;

    #if DEBUG_MODE
    Serial.print("Attempting MQTT connection...");
    #endif

    String clientId = "CSP32_" + String(DEVICE_ID);

    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
        #if DEBUG_MODE
        Serial.println("connected!");
        #endif
    } else {
        #if DEBUG_MODE
        Serial.print("failed, rc=");
        Serial.println(mqttClient.state());
        #endif
    }
}

void publishMQTT() {
    if (!mqttClient.connected() || !currentReading.valid) {
        return;
    }

    // Create JSON document
    StaticJsonDocument<256> doc;
    doc["device_id"] = DEVICE_ID;
    doc["timestamp"] = currentReading.timestamp;
    doc["distance"] = currentReading.distance;
    doc["fill_percentage"] = currentReading.fillPercentage;
    doc["level"] = getLevelString(currentReading.level);
    doc["rssi"] = WiFi.RSSI();

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // Publish to status topic
    String statusTopic = String(MQTT_TOPIC_PREFIX) + String(DEVICE_ID) + "/" + String(MQTT_TOPIC_STATUS);
    mqttClient.publish(statusTopic.c_str(), jsonBuffer);

    #if DEBUG_MODE
    Serial.println("Published to MQTT: " + statusTopic);
    #endif
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void blinkLED() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastLedBlink >= LED_BLINK_INTERVAL) {
        lastLedBlink = currentMillis;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}

void printSensorData() {
    Serial.println("\n--- Sensor Reading ---");
    Serial.print("Timestamp: ");
    Serial.println(currentReading.timestamp);
    Serial.print("Distance: ");
    Serial.print(currentReading.distance);
    Serial.println(" cm");
    Serial.print("Fill Percentage: ");
    Serial.print(currentReading.fillPercentage);
    Serial.println(" %");
    Serial.print("Bin Level: ");
    Serial.println(getLevelString(currentReading.level));
    Serial.print("Valid Reading: ");
    Serial.println(currentReading.valid ? "Yes" : "No");
    Serial.println("---------------------\n");
}
