#include <Servo.h>  // 서보 모터 제어를 위한 라이브러리 포함

Servo crMotor;   // 첫 번째 서보모터 객체
Servo crMotor2;  // 두 번째 서보모터 객체

// 핀 설정
const int BUTTON_PIN = 2;     // 버튼이 연결된 디지털 핀 번호
const int MOTOR_PIN = 9;      // 첫 번째 서보모터 핀
const int MOTOR_PIN2 = 11;    // 두 번째 서보모터 핀

// 디바운싱 관련 변수
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

// 모터 회전 상태
bool isRotating = false;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  crMotor.attach(MOTOR_PIN);     // 첫 번째 모터 연결
  crMotor2.attach(MOTOR_PIN2);   // 두 번째 모터 연결

  crMotor.writeMicroseconds(1500);   // 정지
  crMotor2.writeMicroseconds(1500);  // 정지

  Serial.println("버튼으로 두 개의 모터 ON/OFF 제어 시작");
}

void loop() {
  handleButton();
  delay(10);
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      if (currentButtonState == LOW) {
        toggleMotor();  // 버튼이 눌렸을 때 상태 전환
      }
    }
  }

  lastButtonState = reading;
}

void toggleMotor() {
  isRotating = !isRotating;

  if (isRotating) {
    crMotor.writeMicroseconds(2218.5);   // 시계방향 회전
    crMotor2.writeMicroseconds(3000);  // 시계방향 회전
  } else {
    crMotor.writeMicroseconds(1500);   // 정지
    crMotor2.writeMicroseconds(1500);  // 정지
  }
}