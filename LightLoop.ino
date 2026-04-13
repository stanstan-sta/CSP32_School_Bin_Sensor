#include <Wire.h>
#include <VL53L5CX_api.h>
#include "esp_wifi.h"
#include "esp_bt.h"
#define VL53L5CX_NB_TARGET_PER_ZONE

SET_LOOP_TASK_STACK_SIZE(32 * 1024);

VL53L5CX_Configuration *sensor;

void setup() {
  // Free up WiFi and BT RAM first
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();

  Serial.begin(115200);
  delay(2000);
  Serial.println("\nVL53L5CX Test Starting...");

  Wire.begin(7, 8);
  Wire.setClock(100000);

  sensor = new VL53L5CX_Configuration();
  if (!sensor) {
    Serial.println("❌ Memory allocation failed.");
    while(1);
  }

  sensor->platform.address = VL53L5CX_DEFAULT_I2C_ADDRESS;

  if (vl53l5cx_init(sensor) != VL53L5CX_STATUS_OK) {
    Serial.println("❌ Init failed.");
    while(1);
  }

  Serial.println("✅ Sensor initialised!");

  vl53l5cx_set_resolution(sensor, VL53L5CX_RESOLUTION_8X8);
  vl53l5cx_set_ranging_frequency_hz(sensor, 5);
  vl53l5cx_start_ranging(sensor);

  Serial.println("Ranging started...");
}

void loop() {
  uint8_t ready = 0;
  vl53l5cx_check_data_ready(sensor, &ready);

  if (ready) {
    VL53L5CX_ResultsData results;
    vl53l5cx_get_ranging_data(sensor, &results);

    int center = (results.distance_mm[27] + results.distance_mm[28] +
                  results.distance_mm[35] + results.distance_mm[36]) / 4;

    Serial.print("Center distance: ");
    if (results.target_status[27] != 5)
      Serial.println("Out of range / No target");
    else {
      Serial.print(center);
      Serial.print(" mm  (");
      Serial.print(center / 10.0, 1);
      Serial.println(" cm)");
    }
  }

  delay(100);
}