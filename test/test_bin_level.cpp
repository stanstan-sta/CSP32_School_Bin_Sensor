/**
 * Unit Tests for CSP32 School Bin Sensor
 *
 * Tests the core functionality of the bin level calculation
 * and sensor data processing.
 *
 * Run with: pio test
 */

#include <unity.h>
#include <Arduino.h>

// Mock configuration values for testing
#define BIN_HEIGHT 100
#define EMPTY_THRESHOLD 90
#define LOW_THRESHOLD 70
#define MEDIUM_THRESHOLD 40
#define HIGH_THRESHOLD 20

// Enum for bin levels
enum BinLevel {
    EMPTY,
    LOW,
    MEDIUM,
    HIGH,
    FULL,
    ERROR
};

// Function to test (copied from main for testing)
BinLevel calculateBinLevel(float fillPercentage) {
    float emptyPercentage = 100.0 - fillPercentage;

    if (emptyPercentage > EMPTY_THRESHOLD) return EMPTY;
    if (emptyPercentage > LOW_THRESHOLD) return LOW;
    if (emptyPercentage > MEDIUM_THRESHOLD) return MEDIUM;
    if (emptyPercentage > HIGH_THRESHOLD) return HIGH;
    return FULL;
}

// Test: Empty bin (5% filled = 95% empty)
void test_bin_level_empty(void) {
    BinLevel level = calculateBinLevel(5.0);
    TEST_ASSERT_EQUAL(EMPTY, level);
}

// Test: Low level (25% filled = 75% empty)
void test_bin_level_low(void) {
    BinLevel level = calculateBinLevel(25.0);
    TEST_ASSERT_EQUAL(LOW, level);
}

// Test: Medium level (55% filled = 45% empty)
void test_bin_level_medium(void) {
    BinLevel level = calculateBinLevel(55.0);
    TEST_ASSERT_EQUAL(MEDIUM, level);
}

// Test: High level (75% filled = 25% empty)
void test_bin_level_high(void) {
    BinLevel level = calculateBinLevel(75.0);
    TEST_ASSERT_EQUAL(HIGH, level);
}

// Test: Full bin (95% filled = 5% empty)
void test_bin_level_full(void) {
    BinLevel level = calculateBinLevel(95.0);
    TEST_ASSERT_EQUAL(FULL, level);
}

// Test: Edge case - exactly at threshold (90% empty)
void test_bin_level_threshold_empty(void) {
    BinLevel level = calculateBinLevel(10.0);
    TEST_ASSERT_EQUAL(LOW, level);  // Just below EMPTY threshold
}

// Test: Edge case - exactly at threshold (70% empty)
void test_bin_level_threshold_low(void) {
    BinLevel level = calculateBinLevel(30.0);
    TEST_ASSERT_EQUAL(MEDIUM, level);  // Just below LOW threshold
}

// Test: Fill percentage calculation
void test_fill_percentage_calculation(void) {
    // Test case: sensor reads 50cm, bin is 100cm tall, offset 5cm
    float sensorReading = 50.0;
    float sensorOffset = 5.0;
    float actualDistance = sensorReading - sensorOffset;  // 45cm
    float fillPercentage = 100.0 - ((actualDistance / BIN_HEIGHT) * 100.0);

    // Expected: 55% full (45cm from top of 100cm bin)
    TEST_ASSERT_FLOAT_WITHIN(0.1, 55.0, fillPercentage);
}

// Test: Empty bin calculation
void test_empty_bin_calculation(void) {
    float sensorReading = 95.0;  // Nearly at bottom
    float sensorOffset = 5.0;
    float actualDistance = sensorReading - sensorOffset;  // 90cm
    float fillPercentage = 100.0 - ((actualDistance / BIN_HEIGHT) * 100.0);

    // Expected: 10% full
    TEST_ASSERT_FLOAT_WITHIN(0.1, 10.0, fillPercentage);
}

// Test: Full bin calculation
void test_full_bin_calculation(void) {
    float sensorReading = 10.0;  // Very close to sensor
    float sensorOffset = 5.0;
    float actualDistance = sensorReading - sensorOffset;  // 5cm
    float fillPercentage = 100.0 - ((actualDistance / BIN_HEIGHT) * 100.0);

    // Expected: 95% full
    TEST_ASSERT_FLOAT_WITHIN(0.1, 95.0, fillPercentage);
}

void setup() {
    delay(2000);  // Wait for board to stabilize

    UNITY_BEGIN();

    // Run all tests
    RUN_TEST(test_bin_level_empty);
    RUN_TEST(test_bin_level_low);
    RUN_TEST(test_bin_level_medium);
    RUN_TEST(test_bin_level_high);
    RUN_TEST(test_bin_level_full);
    RUN_TEST(test_bin_level_threshold_empty);
    RUN_TEST(test_bin_level_threshold_low);
    RUN_TEST(test_fill_percentage_calculation);
    RUN_TEST(test_empty_bin_calculation);
    RUN_TEST(test_full_bin_calculation);

    UNITY_END();
}

void loop() {
    // Nothing to do here
}
