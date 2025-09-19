/*
 * RC카 2-아두이노 시스템: 구동용 슬레이브 코드 (TB6612FNG용)
 * [수정]
 * - 전진과 후진의 제어 신호가 반대로 동작하는 문제를 해결하기 위해
 * 전진/후진 시의 digitalWrite 논리를 서로 맞바꿨습니다.
 */

#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 8
#define COMMAND_TIMEOUT 500

// --- 핀 설정 (순서대로 재배치) ---
#define STBY 2 // Standby 핀

// 모터 B (오른쪽)
#define PWMB 3 // PWM 핀
#define BIN1 4
#define BIN2 5

// 모터 A (왼쪽)
#define PWMA 6 // PWM 핀
#define AIN1 7
#define AIN2 8

// 서보 모터 핀
#define SERVO_PIN_A 9
Servo steering_servo;

// --- 전역 제어 변수 ---
volatile uint8_t g_throttle = 0;
volatile uint8_t g_brake = 0;
volatile uint8_t g_steering = 127;
volatile unsigned long g_last_command_time = 0;

// --- 단순 제어 함수 ---
void control_drive(uint8_t throttle, uint8_t brake) {
  int power = throttle - brake; // -255 ~ 255

  if (power > 0) { // 전진
    // 원래 후진이었던 로직을 전진으로 변경
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, power);
    analogWrite(PWMB, power);
  } else if (power < 0) { // 후진
    // 원래 전진이었던 로직을 후진으로 변경
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, abs(power));
    analogWrite(PWMB, abs(power));
  } else { // 정지
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
  }
}

// 서보를 스티어링 값으로 직접 제어하는 함수
void control_steer(uint8_t steer) {
  int servo_angle = map(steer, 0, 255, 0, 180);
  steering_servo.write(servo_angle);
}

// I2C 데이터 수신 이벤트
void receiveEvent(int howMany) {
  if (howMany == 3) {
    g_throttle = Wire.read();
    g_brake = Wire.read();
    g_steering = Wire.read();
    g_last_command_time = millis();
  } else {
    while(Wire.available()) { Wire.read(); }
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  steering_servo.attach(SERVO_PIN_A);

  digitalWrite(STBY, HIGH); // 모터 드라이버 활성화

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  
  g_last_command_time = millis();
  Serial.println("Slave (Simple Control Logic): Ready.");
}

void loop() {
  // 안전 기능
  if (millis() - g_last_command_time > COMMAND_TIMEOUT) {
    g_throttle = 0;
    g_brake = 0;
    g_steering = 127;
  }

  // 받은 값을 각 함수에 직접 전달
  control_drive(g_throttle, g_brake);
  control_steer(g_steering);

  delay(20); 
}
