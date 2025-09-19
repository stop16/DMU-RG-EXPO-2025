#include <Servo.h> // 서보모터 라이브러리를 포함합니다.

Servo continuousServo; // 서보 객체를 생성합니다.

void setup() {
  continuousServo.attach(9); // 서보모터를 아두이노 9번 핀에 연결합니다.
}

void loop() {
  // 1. 속도 45로 1.5초 동안 회전 (시계 방향)
  continuousServo.write(135); // 90보다 큰 값은 반 시계 방향으로 회전합니다. 값이 0에 가까울수록 빠릅니다.
  delay(1500); // 1.5초 동안 대기합니다.

  // 2. 3초 동안 정지
  continuousServo.write(90); // 90은 정지를 의미합니다.
  delay(3000); // 3초 동안 대기합니다.

  // 3. 속도 135로 1.5초 동안 회전 (반시계 방향)
  continuousServo.write(45); // 90보다 작은 값을 시계 방향으로 회전합니다. 값이 180에 가까울수록 빠릅니다.
  delay(1500); // 1.5초 동안 대기합니다.

  continuousServo.write(90); // 90은 정지를 의미합니다.
  delay(3000); // 3초 동안 대기합니다.
}