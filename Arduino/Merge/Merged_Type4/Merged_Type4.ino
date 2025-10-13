/**
 * 2025 RG EXPO Code (Merge ver) Type 4
 * Components
 * 1. 1x ToF Sensor

 * Scenario
 * 1. Send '9' when received 'q' (Query for MCU type)
 * 2. Send 'o' when ToF detected < TOF_DISTANCE_THRESHOLD (mm)
 */

#include <Wire.h>
#include <VL53L0X.h>

#define MY_TYPE 9

// ToF
VL53L0X ToF_sensor;

// 히스테리시스 임계
static const uint16_t THRESH_LOW  = 200;  // 트리거 기준 (이 값 미만으로 내려갈 때 단발성으로 'o' 출력)
static const uint16_t THRESH_HIGH = 220;  // 재무장 기준 (이 값 초과하면 다시 출력 준비)

void setup() {
  // UART
  Serial.begin(115200);

  // ToF
  Wire.begin();
  ToF_sensor.setTimeout(500);
  if (!ToF_sensor.init()) {
    while (1) {}               // 초기화 실패 시 정지
  } else {}
  ToF_sensor.startContinuous();

  // 타입 식별 출력
  Serial.println(MY_TYPE);
}

void loop() {
  // 무장/해제 상태
  //  - armed=true: 다음에 거리값이 THRESH_LOW 미만으로 "진입"하면 'o' 1회 송신 후 해제
  //  - armed=false: 거리값이 THRESH_HIGH 초과로 "이탈"하면 다시 무장
  static bool armed = false;
  static bool initialized = false;

  // 현재 거리 읽기
  uint16_t d = ToF_sensor.readRangeContinuousMillimeters();
  bool valid = !(ToF_sensor.timeoutOccurred() || d == 0 || d == 65535);

  // 첫 루프 초기화:
  //  시작 시 이미 가까이에 있으면(LOW 미만) 바로 쏘지 않도록
  //  "멀리 있음(HIGH 초과)"일 때만 무장 상태로 시작
  if (!initialized) {
    initialized = true;
    if (valid && d > THRESH_HIGH) {
      armed = true;
    } else {
      armed = false;
    }
  }

  // 유효값일 때만 상태 전이 수행
  if (valid) {
    // 발사 조건: 무장 상태에서 LOW 미만으로 "진입"한 순간
    if (armed && d < THRESH_LOW) {
      Serial.println('o');   // 1회 송신
      armed = false;         // 즉시 해제(연타 방지)
    }
    // 재무장 조건: 해제 상태에서 HIGH 초과로 "이탈"한 순간
    else if (!armed && d > THRESH_HIGH) {
      armed = true;
    }
    // LOW~HIGH 구간에서는 상태 유지 → 노이즈에 대한 채터링 방지
  }
  // 유효하지 않으면(타임아웃/에러) 상태 유지

  // 호스트 쿼리 처리
  if (Serial.available()) {
    char input = (char)Serial.read();
    if (input == 'q') {
      Serial.println(MY_TYPE);
    }
  }
}
