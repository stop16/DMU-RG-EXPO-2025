/**
 * 2025 RG EXPO Code (Merge ver) Type 2
 * Components
 * 1. 1x ToF Sensor

 * Scenario
 * 1. Send '9' when received 'q' (Query for MCU type)
 * 2. Send 'o' when ToF detected < TOF_DISTANCE_THRESHOLD (mm)
 */

#include <Wire.h>
#include <VL53L0X.h>

#define MY_TYPE 9

// ToF Related
VL53L0X ToF_sensor;

#define TOF_DISTANCE_THRESHOLD  50
#define TOF_NOT_DETECTED        0
#define TOF_DETECTED            1

uint8_t tof_check_detection(void);

void setup() {
  // UART Setup
  Serial.begin(115200);
  // ToF Setup
  Wire.begin();
  ToF_sensor.setTimeout(500);
  if (!ToF_sensor.init())
  {
    while (1) {}
  }
  else Serial.println("TOF INIT");
  ToF_sensor.startContinuous();
  Serial.println(MY_TYPE);
}

void loop() {
  uint8_t flag_tof_detected = tof_check_detection();
  char input;
  if (flag_tof_detected) Serial.println('o');
  if (Serial.available()) {
    input = Serial.read();
    switch (input) {
      case 'q': // Type query -> Send type number
        Serial.println(MY_TYPE);
        break;
      default:
        break;
    }
  }
}

/**
 * ToF Detect function
 * Return Detected when distance < threshold
 */
uint8_t tof_check_detection(void)
{
    if (ToF_sensor.readRangeContinuousMillimeters() < TOF_DISTANCE_THRESHOLD) {
        return TOF_DETECTED;
    }
    else {
        return TOF_NOT_DETECTED;
    }
}
